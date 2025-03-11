#include "RenderObject.h"
#include "RE/N/NiCamera.h"
#include "RE/P/PlayerCamera.h"

namespace SoundFX {

    bool
        RenderObject::WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float &depth) {
        const auto *camera = RE::PlayerCamera::GetSingleton();
        if (!camera) {
            return false;
        }

        const auto root = camera->cameraRoot.get();
        if (!root || root->GetChildren().empty()) {
            return false;
        }

        if (std::strcmp(root->GetChildren()[0]->GetRTTI()->name, "NiCamera") == 0) {
            const auto niCamera = static_cast<RE::NiCamera *>(root->GetChildren()[0].get());

            const auto &[worldToCam] = niCamera->GetRuntimeData();
            const auto &viewport     = niCamera->GetRuntimeData2().port;

            float      x, y, z;
            const bool visible =
                RE::NiCamera::WorldPtToScreenPt3(worldToCam, viewport, worldPos, x, y, z, 0.0001f);

            if (!visible || z <= 0.0f) {
                return false;  // Is Behind Camera
            }

            const ImGuiIO &io = ImGui::GetIO();
            screenPos.x       = x * io.DisplaySize.x;
            screenPos.y       = (1.0f - y) * io.DisplaySize.y;
            depth             = z;

            return true;
        }

        return false;
    }

    bool
        RenderObject::IsObjectObstructed(const RE::NiPoint3 &position,
                                         float               radius,
                                         int                 numSamples,
                                         float               obstructionThreshold) {
        auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->GetParentCell() || !player->GetParentCell()->GetbhkWorld()) {
            return false;
        }

        RE::NiPoint3 cameraPos       = RE::PlayerCamera::GetSingleton()->pos;
        auto        *bhkWorld        = player->GetParentCell()->GetbhkWorld();
        int          obstructedCount = 0;
        int          totalChecks     = numSamples + 1;

        RE::bhkPickData pickData;
        pickData.rayInput.from = cameraPos * RE::bhkWorld::GetWorldScale();

        // Check the Center Point first
        pickData.rayInput.to = position * RE::bhkWorld::GetWorldScale();
        bhkWorld->PickObject(pickData);

        if (pickData.rayOutput.HasHit()) {
            if (auto collidable = pickData.rayOutput.rootCollidable) {
                auto ref = RE::TESHavokUtilities::FindCollidableRef(*collidable);
                if (ref) {
                    obstructedCount++;
                }
            }
        }

        // Check several Points on the Circle
        for (int i = 0; i < numSamples; ++i) {
            float angle = static_cast<float>(i) / static_cast<float>(numSamples) * 2.0f
                        * std::numbers::pi_v<float>;

            RE::NiPoint3 point = {position.x + radius * std::cos(angle),
                                  position.y + radius * std::sin(angle),
                                  position.z};

            pickData.rayInput.to = point * RE::bhkWorld::GetWorldScale();
            bhkWorld->PickObject(pickData);

            if (pickData.rayOutput.HasHit()) {
                if (auto collidable = pickData.rayOutput.rootCollidable) {
                    auto ref = RE::TESHavokUtilities::FindCollidableRef(*collidable);
                    if (ref) {
                        obstructedCount++;
                    }
                }
            }
        }

        float obstructionPercentage =
            static_cast<float>(obstructedCount) / static_cast<float>(totalChecks);

        return obstructionPercentage >= obstructionThreshold;
    }

    void
        RenderObject::Draw3DCircle(const RE::NiPoint3 &center,
                                   float               radius,
                                   ImDrawList         *drawList,
                                   ImU32               color) {
        constexpr int   numSegments    = 100;
        constexpr float angleIncrement = 2.0f * std::numbers::pi_v<float> / numSegments;

        drawList->PathClear();
        for (int i = 0; i < numSegments; ++i) {
            const float angle = static_cast<float>(i) * angleIncrement;

            RE::NiPoint3 point = {
                center.x + radius * std::cos(angle), center.y + radius * std::sin(angle), center.z};

            ImVec2 screenPos;

            if (float depth; WorldToScreen(point, screenPos, depth)) {
                drawList->PathLineTo(screenPos);
            }
        }
        drawList->PathFillConvex(color);
    }

    void
        RenderObject::Draw3DCircleOutline(const RE::NiPoint3 &center,
                                          float               radius,
                                          ImDrawList         *drawList,
                                          ImU32               color,
                                          float               thickness) {
        constexpr int   numSegments    = 100;
        constexpr float angleIncrement = 2.0f * std::numbers::pi_v<float> / numSegments;

        drawList->PathClear();
        ImVec2 firstScreenPos;
        bool   firstSet = false;

        for (int i = 0; i < numSegments; ++i) {
            const float angle = static_cast<float>(i) * angleIncrement;

            RE::NiPoint3 point = {
                center.x + radius * std::cos(angle), center.y + radius * std::sin(angle), center.z};

            ImVec2 screenPos;
            float  depth;

            if (WorldToScreen(point, screenPos, depth)) {
                drawList->PathLineTo(screenPos);
                if (!firstSet) {
                    firstScreenPos = screenPos;
                    firstSet       = true;
                }
            }
        }

        // Closes the circle by connecting the last point to the first point
        if (firstSet) {
            drawList->PathLineTo(firstScreenPos);
        }

        drawList->PathStroke(color, 0, thickness);
    }

    void
        RenderObject::Draw3DSphere(const RE::NiPoint3 &center,
                                   float               radius,
                                   ImDrawList         *drawList,
                                   ImU32               color) {
        constexpr int numCircles = 35;  // Number of horizontal circles
        for (int i = 0; i <= numCircles; ++i) {
            const float angle =
                static_cast<float>(i) / static_cast<float>(numCircles) * std::numbers::pi_v<float>;
            const float r = radius * std::sin(angle);
            float       z = center.z + radius * std::cos(angle);

            Draw3DCircle({center.x, center.y, z}, r, drawList, color);
        }
    }
}
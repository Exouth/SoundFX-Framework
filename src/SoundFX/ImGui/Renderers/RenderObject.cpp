#include "RenderObject.h"
#include <numbers>

namespace SoundFX {

    bool
        RenderObject::WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float *depth) {
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

            if (depth && (!visible || z <= 0.0f)) {
                return false;  // Is Behind Camera
            }

            const ImGuiIO &io = ImGui::GetIO();
            screenPos         = {x * io.DisplaySize.x, (1.0f - y) * io.DisplaySize.y};

            if (depth) {
                *depth = z;
            }

            return true;
        }

        return false;
    }

    bool
        RenderObject::IsObjectObstructed(const RE::NiPoint3 &position,
                                         float               radius,
                                         int                 numSamples,
                                         float               obstructionThreshold) {
        if (const auto player = RE::PlayerCharacter::GetSingleton();
            !player || !player->GetParentCell() || !player->GetParentCell()->GetbhkWorld()) {
            return false;
        }

        const RE::NiPoint3 cameraPos       = RE::PlayerCamera::GetSingleton()->pos;
        int                obstructedCount = 0;
        const int          totalChecks     = numSamples + 1;

        // Check the Center Point first
        if (PickObject(cameraPos, position)) {
            obstructedCount++;
        }

        // Check several Points on the Circle
        for (int i = 0; i < numSamples; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(numSamples) * 2.0f
                              * std::numbers::pi_v<float>;

            RE::NiPoint3 point = {position.x + radius * std::cos(angle),
                                  position.y + radius * std::sin(angle),
                                  position.z};

            if (PickObject(cameraPos, point)) {
                obstructedCount++;
            }
        }

        const float obstructionPercentage =
            static_cast<float>(obstructedCount) / static_cast<float>(totalChecks);

        return obstructionPercentage >= obstructionThreshold;
    }

    bool
        RenderObject::PickObject(const RE::NiPoint3 &from, const RE::NiPoint3 &to) {
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player || !player->GetParentCell() || !player->GetParentCell()->GetbhkWorld()) {
            return false;
        }

        auto *bhkWorld = player->GetParentCell()->GetbhkWorld();

        RE::bhkPickData pickData;
        pickData.rayInput.from = from * RE::bhkWorld::GetWorldScale();
        pickData.rayInput.to   = to * RE::bhkWorld::GetWorldScale();
        bhkWorld->PickObject(pickData);

        if (pickData.rayOutput.HasHit()) {
            if (const auto collidable = pickData.rayOutput.rootCollidable) {
                const auto ref = RE::TESHavokUtilities::FindCollidableRef(*collidable);
                return ref != nullptr;
            }
        }

        return false;
    }

    void
        RenderObject::Draw3DCircle(const RE::NiPoint3 &center,
                                   float               radius,
                                   ImDrawList         *drawList,
                                   ImU32               color,
                                   int                 numSegmentsCircle) {
        const float angleIncrement =
            2.0f * std::numbers::pi_v<float> / static_cast<float>(numSegmentsCircle);

        drawList->PathClear();
        for (int i = 0; i < numSegmentsCircle; ++i) {
            const float angle = static_cast<float>(i) * angleIncrement;

            RE::NiPoint3 point = {
                center.x + radius * std::cos(angle), center.y + radius * std::sin(angle), center.z};

            ImVec2 screenPos;

            if (float depth; WorldToScreen(point, screenPos, &depth)) {
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
                                          float               thickness,
                                          int                 numSegmentsCircle) {
        const float angleIncrement =
            2.0f * std::numbers::pi_v<float> / static_cast<float>(numSegmentsCircle);

        drawList->PathClear();
        ImVec2 firstScreenPos;
        bool   firstSet = false;

        for (int i = 0; i < numSegmentsCircle; ++i) {
            const float angle = static_cast<float>(i) * angleIncrement;

            RE::NiPoint3 point = {
                center.x + radius * std::cos(angle), center.y + radius * std::sin(angle), center.z};

            ImVec2 screenPos;
            float  depth;

            if (WorldToScreen(point, screenPos, &depth)) {
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
                                   ImU32               color,
                                   int                 numSegmentsSphere) {
        for (int i = 0; i <= numSegmentsSphere; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(numSegmentsSphere)
                              * std::numbers::pi_v<float>;
            const float r = radius * std::sin(angle);
            float       z = center.z + radius * std::cos(angle);

            Draw3DCircle({center.x, center.y, z}, r, drawList, color, numSegmentsSphere);
        }
    }

    // The tracer itself is likely somewhat inaccurate, so I have added a scaling factor that
    // stretches the projection outward. While this does not fully resolve the underlying issue, it
    // ensures that the tracer remains reasonably functional. If I have more time, I will look into
    // it further, but for now, it serves its purpose well for debugging.
    void
        RenderObject::DrawTracerLine(const RE::NiPoint3 &markerPos,
                                     const RE::NiPoint3 &playerPos,
                                     ImDrawList         *drawList,
                                     ImU32               color,
                                     float               thickness) {
        if (!drawList)
            return;

        ImVec2     screenStart, screenEnd;
        float      depth;
        const bool startVisible = WorldToScreen(playerPos, screenStart, &depth);
        const bool endVisible   = WorldToScreen(markerPos, screenEnd, &depth);

        if (!endVisible) {
            const ImGuiIO &io = ImGui::GetIO();
            const ImVec2   screenCenter(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);

            if (ImVec2 directionOnScreen; WorldToScreen(markerPos, directionOnScreen)) {
                directionOnScreen.x -= screenCenter.x;
                directionOnScreen.y -= screenCenter.y;

                if (const float length = std::hypot(directionOnScreen.x, directionOnScreen.y);
                    length > 0) {
                    directionOnScreen.x /= length;
                    directionOnScreen.y /= length;

                    const float scale =
                        std::min(
                            std::abs((directionOnScreen.x > 0 ? io.DisplaySize.x - screenCenter.x
                                                              : screenCenter.x)
                                     / directionOnScreen.x),
                            std::abs((directionOnScreen.y > 0 ? io.DisplaySize.y - screenCenter.y
                                                              : screenCenter.y)
                                     / directionOnScreen.y))
                        * 1000.0f;

                    screenEnd.x = screenCenter.x + directionOnScreen.x * scale;
                    screenEnd.y = screenCenter.y + directionOnScreen.y * scale;
                }
            }
        }

        if (startVisible) {
            drawList->AddLine(screenStart, screenEnd, color, thickness);
        } else {
            // Fallback
            const ImGuiIO &io = ImGui::GetIO();
            drawList->AddLine(
                {io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f}, screenEnd, color, thickness);
        }
    }

    void
        RenderObject::DrawTextAboveSphere(const RE::NiPoint3 &center,
                                          float               radius,
                                          ImU32               color,
                                          ImDrawList         *drawList,
                                          const std::string  &name,
                                          const std::string  &eventType,
                                          const std::string  &soundEffect,
                                          bool                isObstructed) {
        if (!drawList) {
            return;
        }

        ImVec2 screenPos;
        float  depth;
        if (WorldToScreen({center.x, center.y, center.z + radius + 10.0f}, screenPos, &depth)) {
            const ImVec2 textSizeName       = ImGui::CalcTextSize(name.c_str());
            const ImVec2 textSizeEvent      = ImGui::CalcTextSize(eventType.c_str());
            const ImVec2 textSizeEffect     = ImGui::CalcTextSize(soundEffect.c_str());
            const ImVec2 textSizeObstructed = ImGui::CalcTextSize("Is Obstructed");

            constexpr float bgPadding = 5.0f;
            float bgHeight = textSizeName.y + textSizeEvent.y + textSizeEffect.y + 3 * bgPadding;
            const float bgWidth =
                std::max({textSizeName.x, textSizeEvent.x, textSizeEffect.x, textSizeObstructed.x})
                + 2 * bgPadding;

            if (isObstructed) {
                bgHeight += textSizeObstructed.y + bgPadding;
            }

            constexpr ImU32 bgColor = IM_COL32(0, 0, 0, 150);
            drawList->AddRectFilled(ImVec2(screenPos.x - bgWidth * 0.5f, screenPos.y - bgHeight),
                                    ImVec2(screenPos.x + bgWidth * 0.5f, screenPos.y),
                                    bgColor,
                                    5.0f);

            float textPosY = screenPos.y - bgHeight + bgPadding;

            if (isObstructed) {
                drawList->AddText(ImVec2(screenPos.x - textSizeObstructed.x * 0.5f, textPosY),
                                  IM_COL32(255, 0, 0, 255),
                                  "Is Obstructed");
                textPosY += textSizeObstructed.y + bgPadding;
            }

            drawList->AddText(
                ImVec2(screenPos.x - textSizeName.x * 0.5f, textPosY), color, name.c_str());
            textPosY += textSizeName.y;

            drawList->AddText(
                ImVec2(screenPos.x - textSizeEvent.x * 0.5f, textPosY), color, eventType.c_str());
            textPosY += textSizeEvent.y;

            drawList->AddText(ImVec2(screenPos.x - textSizeEffect.x * 0.5f, textPosY),
                              color,
                              soundEffect.c_str());
        }
    }
}
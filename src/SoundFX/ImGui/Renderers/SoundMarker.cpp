#include "SoundMarker.h"
#include "RE/P/PlayerCamera.h"
#include "RenderObject.h"

namespace SoundFX {
    bool  SoundMarker::showSoundMarkers      = false;
    bool  SoundMarker::distanceFilterEnabled = true;
    float SoundMarker::maxRenderDistance     = 4000.0f;
    float SoundMarker::soundRadius           = 100.0f;

    void
        SoundMarker::Render(ImDrawList *drawList) {
        if (!showSoundMarkers)
            return;

        const auto *camera = RE::PlayerCamera::GetSingleton();
        if (!camera) {
            return;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        // Testing Purpose (Delete Later)
        const std::vector<RE::NiPoint3> testPositions = {
            {player->GetPosition()}, {7729.8643f, -68789.37f, 4515.15f}, {300.0f, 300.0f, 300.0f}};

        for (const auto &pos : testPositions) {
            ImVec2 screenPos;
            float  depth;

            const float distance = camera->pos.GetDistance(pos);
            if (distanceFilterEnabled && distance > maxRenderDistance) {
                continue;
            }

            if (RenderObject::WorldToScreen(pos, screenPos, depth)) {
                const float size = CalculateMarkerSize(distance);

                // Draws a border at the radius
                RenderObject::Draw3DCircleOutline(
                    pos, soundRadius, drawList, IM_COL32(0, 0, 0, 255), 5.0f);

                // Draws Radius/Reach of Sounds
                RenderObject::Draw3DCircle(pos, soundRadius, drawList, IM_COL32(0, 0, 255, 128));

                // Draws Marker of Sounds
                RenderObject::Draw3DSphere(pos, size, drawList, IM_COL32(255, 255, 0, 255));
            }
        }
        //
    }

    void
        SoundMarker::ToggleVisibility() {
        showSoundMarkers = !showSoundMarkers;
        spdlog::debug("Sound markers visibility toggled: {}", showSoundMarkers);
    }

    bool
        SoundMarker::IsVisible() {
        return showSoundMarkers;
    }

    void
        SoundMarker::SetMaxRenderDistance(float distance) {
        maxRenderDistance = distance;
    }

    void
        SoundMarker::EnableDistanceFilter(bool enable) {
        distanceFilterEnabled = enable;
    }

    bool
        SoundMarker::IsDistanceFilterEnabled() {
        return distanceFilterEnabled;
    }

    float
        SoundMarker::GetMaxRenderDistance() {
        return maxRenderDistance;
    }

    void
        SoundMarker::SetSoundRadius(float radius) {
        soundRadius = radius;
    }
}
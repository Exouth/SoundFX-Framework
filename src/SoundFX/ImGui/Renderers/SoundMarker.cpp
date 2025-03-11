#include "SoundMarker.h"
#include "ImGui/Settings/DefaultSettings.h"
#include "RE/P/PlayerCamera.h"
#include "RenderObject.h"

namespace SoundFX {
    bool  SoundMarker::showSoundMarkers         = DefaultSettings::GetShowSoundMarkers();
    bool  SoundMarker::distanceFilterEnabled    = DefaultSettings::GetDistanceFilterEnabled();
    float SoundMarker::maxRenderDistance        = DefaultSettings::GetMaxRenderDistance();
    float SoundMarker::soundRadius              = DefaultSettings::GetSoundRadius();
    bool  SoundMarker::obstructionEffectEnabled = DefaultSettings::GetObstructionEffectEnabled();
    float SoundMarker::obstructionThreshold     = DefaultSettings::GetObstructionThreshold();
    bool  SoundMarker::radiusIndicator          = DefaultSettings::GetRadiusIndicator();
    float SoundMarker::radiusOutlineThickness   = DefaultSettings::GetRadiusOutlineThickness();

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

                const float localObstructionThreshold = GetObstructionThreshold();

                const bool isObstructed = IsObstructionEffectEnabled()
                                       && RenderObject::IsObjectObstructed(
                                              pos, soundRadius, 16, localObstructionThreshold);

                const ImU32 color =
                    isObstructed ? IM_COL32(128, 128, 128, 255) : IM_COL32(255, 255, 0, 255);
                const ImU32 circleColor =
                    isObstructed ? IM_COL32(64, 64, 64, 128) : IM_COL32(0, 0, 255, 128);

                if (IsRadiusIndicatorEnabled()) {
                    // Draws a border at the radius
                    RenderObject::Draw3DCircleOutline(
                        pos, soundRadius, drawList, IM_COL32(0, 0, 0, 255), radiusOutlineThickness);

                    // Draws Radius/Reach of Sounds
                    RenderObject::Draw3DCircle(pos, soundRadius, drawList, circleColor);
                }

                // Draws Marker of Sounds
                RenderObject::Draw3DSphere(pos, size, drawList, color);
            }
        }
        //
    }

    void
        SoundMarker::ToggleVisibility(std::optional<bool> forceState) {
        if (forceState.has_value()) {
            showSoundMarkers = forceState.value();
        } else {
            showSoundMarkers = !showSoundMarkers;
        }

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

    void
        SoundMarker::EnableObstructionEffect(bool enable) {
        obstructionEffectEnabled = enable;
    }

    bool
        SoundMarker::IsObstructionEffectEnabled() {
        return obstructionEffectEnabled;
    }

    void
        SoundMarker::SetObstructionThreshold(float threshold) {
        obstructionThreshold = std::clamp(threshold, 0.0f, 1.0f);
    }

    float
        SoundMarker::GetObstructionThreshold() {
        return obstructionThreshold;
    }

    void
        SoundMarker::SetRadiusIndicator(bool enable) {
        radiusIndicator = enable;
    }

    bool
        SoundMarker::IsRadiusIndicatorEnabled() {
        return radiusIndicator;
    }

    void
        SoundMarker::SetRadiusOutlineThickness(float thickness) {
        radiusOutlineThickness = thickness;
    }

    float
        SoundMarker::GetRadiusOutlineThickness() {
        return radiusOutlineThickness;
    }
}
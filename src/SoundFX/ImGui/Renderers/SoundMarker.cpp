#include "SoundMarker.h"
#include "ImGui/Settings/DefaultSettings.h"
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
    bool  SoundMarker::tracers                  = DefaultSettings::GetTracers();
    int   SoundMarker::maxSoundMarkers          = DefaultSettings::GetMaxSoundMarkers();

    void
        SoundMarker::Render(ImDrawList *drawList) {
        if (!showSoundMarkers || !drawList) {
            return;
        }

        const auto *camera = RE::PlayerCamera::GetSingleton();
        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!camera || !player) {
            return;
        }

        std::vector<RE::NiPoint3> soundPositions = GetActiveSoundPositions();
        if (soundPositions.empty()) {
            return;
        }

        std::ranges::sort(soundPositions,
                          [camera](const RE::NiPoint3 &a, const RE::NiPoint3 &b) {
                              return camera->pos.GetDistance(a) < camera->pos.GetDistance(b);
                          });

        if (maxSoundMarkers != -1 && soundPositions.size() > static_cast<size_t>(maxSoundMarkers)) {
            soundPositions.resize(maxSoundMarkers);
        }

        for (const auto &soundPos : soundPositions) {
            ProcessSoundMarker(soundPos, player->GetPosition(), drawList, camera->pos);
        }
    }

    std::vector<RE::NiPoint3>
        SoundMarker::GetActiveSoundPositions() {

        // Currently Test Position for debugging purpose
        return {{7729.8643f, -68789.37f, 4515.15f},
                {8029.8643f, -69089.37f, 4815.15f},
                {7429.8643f, -68489.37f, 4215.15f},
                {7729.8643f, -68089.37f, 4515.15f},
                {8129.8643f, -68789.37f, 4915.15f},
                {7629.8643f, -69089.37f, 4415.15f},
                {7829.8643f, -68489.37f, 4715.15f},
                {7529.8643f, -68089.37f, 4315.15f},
                {7929.8643f, -68789.37f, 4615.15f},
                {7729.8643f, -69089.37f, 4515.15f},
                {7629.8643f, -68489.37f, 4415.15f},
                {7829.8643f, -68089.37f, 4715.15f},
                {7529.8643f, -68789.37f, 4315.15f},
                {7929.8643f, -69089.37f, 4615.15f},
                {7729.8643f, -68489.37f, 4515.15f}};
    }

    void
        SoundMarker::ProcessSoundMarker(const RE::NiPoint3 &soundPos,
                                        const RE::NiPoint3 &playerPos,
                                        ImDrawList         *drawList,
                                        const RE::NiPoint3 &cameraPos) {
        if (!drawList) {
            return;
        }

        const float distance = cameraPos.GetDistance(soundPos);
        if (distanceFilterEnabled && distance > maxRenderDistance) {
            return;
        }

        const bool isObstructed =
            obstructionEffectEnabled
            && RenderObject::IsObjectObstructed(soundPos, soundRadius, 16, obstructionThreshold);

        const ImU32 markerColor =
            isObstructed ? IM_COL32(128, 128, 128, 255) : IM_COL32(255, 255, 0, 255);
        const ImU32 tracerColor =
            isObstructed ? IM_COL32(64, 64, 64, 128) : IM_COL32(0, 0, 255, 128);

        if (tracers) {
            RenderObject::DrawTracerLine(soundPos, playerPos, drawList, tracerColor, 2.0f);
        }

        ImVec2 screenPos;
        float  depth;
        if (RenderObject::WorldToScreen(soundPos, screenPos, &depth)) {
            DrawSoundMarker(soundPos, distance, drawList, markerColor, tracerColor);
        }
    }

    void
        SoundMarker::DrawSoundMarker(const RE::NiPoint3 &soundPos,
                                     float               distance,
                                     ImDrawList         *drawList,
                                     ImU32               markerColor,
                                     ImU32               tracerColor) {
        if (!drawList) {
            return;
        }

        const float markerSize = CalculateMarkerSize(distance);

        if (radiusIndicator) {
            RenderObject::Draw3DCircleOutline(
                soundPos, soundRadius, drawList, IM_COL32(0, 0, 0, 255), radiusOutlineThickness);
            RenderObject::Draw3DCircle(soundPos, soundRadius, drawList, tracerColor);
        }

        RenderObject::Draw3DSphere(soundPos, markerSize, drawList, markerColor);
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

    void
        SoundMarker::EnableTracers(bool enable) {
        tracers = enable;
    }

    bool
        SoundMarker::IsTracersEnabled() {
        return tracers;
    }

    void
        SoundMarker::SetMaxSoundMarkers(int maxMarkers) {
        maxSoundMarkers = maxMarkers;
    }
}
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
    int   SoundMarker::numSegmentsCircle        = DefaultSettings::GetNumSegmentsCircle();
    int   SoundMarker::numSegmentsSphere        = DefaultSettings::GetNumSegmentsSphere();
    bool  SoundMarker::textHover                = DefaultSettings::GetTextHover();

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

        std::vector<std::tuple<RE::NiPoint3, std::string, std::string>> soundPositions =
            GetActiveSoundPositions();
        if (soundPositions.empty()) {
            return;
        }

        std::ranges::sort(soundPositions, [player](const auto &a, const auto &b) {
            return player->GetPosition().GetDistance(std::get<0>(a))
                 < player->GetPosition().GetDistance(std::get<0>(b));
        });

        if (maxSoundMarkers != -1 && soundPositions.size() > static_cast<size_t>(maxSoundMarkers)) {
            soundPositions.resize(maxSoundMarkers);
        }

        for (const auto &[soundPos, name, soundEffect] : soundPositions) {
            ProcessSoundMarker(
                soundPos, player->GetPosition(), drawList, camera->pos, name, soundEffect);
        }
    }

    std::vector<std::tuple<RE::NiPoint3, std::string, std::string>>
        SoundMarker::GetActiveSoundPositions() {

        // Currently Test Position for debugging purpose
        return {{{7729.8643f, -68789.37f, 4515.15f}, "Sound1", "Effect1"},
                {{8029.8643f, -69089.37f, 4815.15f}, "Sound2", "Effect2"},
                {{7429.8643f, -68489.37f, 4215.15f}, "Sound3", "Effect3"},
                {{7729.8643f, -68089.37f, 4515.15f}, "Sound4", "Effect4"},
                {{8129.8643f, -68789.37f, 4915.15f}, "Sound5", "Effect5"},
                {{7629.8643f, -69089.37f, 4415.15f}, "Sound6", "Effect6"},
                {{7829.8643f, -68489.37f, 4715.15f}, "Sound7", "Effect7"},
                {{7529.8643f, -68089.37f, 4315.15f}, "Sound8", "Effect8"},
                {{7929.8643f, -68789.37f, 4615.15f}, "Sound9", "Effect9"},
                {{7729.8643f, -69089.37f, 4515.15f}, "Sound10", "Effect10"},
                {{7629.8643f, -68489.37f, 4415.15f}, "Sound11", "Effect11"},
                {{7829.8643f, -68089.37f, 4715.15f}, "Sound12", "Effect12"},
                {{7529.8643f, -68789.37f, 4315.15f}, "Sound13", "Effect13"},
                {{7929.8643f, -69089.37f, 4615.15f}, "Sound14", "Effect14"},
                {{7729.8643f, -68489.37f, 4515.15f}, "Sound15", "Effect15"}};
    }

    void
        SoundMarker::ProcessSoundMarker(const RE::NiPoint3 &soundPos,
                                        const RE::NiPoint3 &playerPos,
                                        ImDrawList         *drawList,
                                        const RE::NiPoint3 &cameraPos,
                                        const std::string  &name,
                                        const std::string  &soundEffect) {
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
        const ImU32 textColor =
            isObstructed ? IM_COL32(192, 192, 192, 255) : IM_COL32(255, 255, 255, 255);

        if (tracers) {
            RenderObject::DrawTracerLine(soundPos, playerPos, drawList, tracerColor, 2.0f);
        }

        ImVec2 screenPos;
        float  depth;
        if (RenderObject::WorldToScreen(soundPos, screenPos, &depth)) {
            DrawSoundMarker(soundPos,
                            distance,
                            drawList,
                            markerColor,
                            tracerColor,
                            textColor,
                            name,
                            soundEffect);
        }
    }

    void
        SoundMarker::DrawSoundMarker(const RE::NiPoint3 &soundPos,
                                     float               distance,
                                     ImDrawList         *drawList,
                                     ImU32               markerColor,
                                     ImU32               tracerColor,
                                     ImU32               textColor,
                                     const std::string  &name,
                                     const std::string  &soundEffect) {
        if (!drawList) {
            return;
        }

        const float markerSize = CalculateMarkerSize(distance);

        if (radiusIndicator) {
            RenderObject::Draw3DCircleOutline(soundPos,
                                              soundRadius,
                                              drawList,
                                              IM_COL32(0, 0, 0, 255),
                                              radiusOutlineThickness,
                                              numSegmentsCircle);
            RenderObject::Draw3DCircle(
                soundPos, soundRadius, drawList, tracerColor, numSegmentsCircle);
        }

        RenderObject::Draw3DSphere(soundPos, markerSize, drawList, markerColor, numSegmentsSphere);

        if (textHover) {
            RenderObject::DrawTextAboveSphere(
                soundPos, markerSize, textColor, drawList, name, soundEffect);
        }
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

    void
        SoundMarker::SetNumSegmentsCircle(int numSegments) {
        numSegmentsCircle = numSegments;
    }

    int
        SoundMarker::GetNumSegmentsCircle() {
        return numSegmentsCircle;
    }

    void
        SoundMarker::SetNumSegmentsSphere(int numSegments) {
        numSegmentsSphere = numSegments;
    }

    int
        SoundMarker::GetNumSegmentsSphere() {
        return numSegmentsSphere;
    }

    void
        SoundMarker::EnableTextHover(bool enable) {
        textHover = enable;
    }

    bool
        SoundMarker::IsTextHoverEnabled() {
        return textHover;
    }
}
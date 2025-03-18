#include "SoundMarker.h"
#include "ImGui/Settings/DefaultSettings.h"
#include "RenderObject.h"
#include "Sound/SoundManager.h"

namespace SoundFX {
    bool   SoundMarker::showSoundMarkers         = DefaultSettings::GetShowSoundMarkers();
    bool   SoundMarker::distanceFilterEnabled    = DefaultSettings::GetDistanceFilterEnabled();
    float  SoundMarker::maxRenderDistance        = DefaultSettings::GetMaxRenderDistance();
    bool   SoundMarker::obstructionEffectEnabled = DefaultSettings::GetObstructionEffectEnabled();
    float  SoundMarker::obstructionThreshold     = DefaultSettings::GetObstructionThreshold();
    bool   SoundMarker::radiusIndicator          = DefaultSettings::GetRadiusIndicator();
    float  SoundMarker::radiusOutlineThickness   = DefaultSettings::GetRadiusOutlineThickness();
    bool   SoundMarker::tracers                  = DefaultSettings::GetTracers();
    int    SoundMarker::maxSoundMarkers          = DefaultSettings::GetMaxSoundMarkers();
    int    SoundMarker::numSegmentsCircle        = DefaultSettings::GetNumSegmentsCircle();
    int    SoundMarker::numSegmentsSphere        = DefaultSettings::GetNumSegmentsSphere();
    bool   SoundMarker::textHover                = DefaultSettings::GetTextHover();
    ImVec4 SoundMarker::markerColor              = DefaultSettings::GetMarkerColor();
    ImVec4 SoundMarker::radiusIndicatorColor     = DefaultSettings::GetRadiusIndicatorColor();
    ImVec4 SoundMarker::tracerColor              = DefaultSettings::GetTracerColor();
    ImVec4 SoundMarker::textHoverColor           = DefaultSettings::GetTextHoverColor();
    bool   SoundMarker::enableRadiusOutlineColorChange =
        DefaultSettings::GetEnableRadiusOutlineColorChange();
    ImVec4 SoundMarker::radiusOutlineColor = DefaultSettings::GetRadiusOutlineColor();

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

        auto activeSounds = SoundManager::GetActiveSounds();
        if (activeSounds.empty()) {
            return;
        }

        std::ranges::sort(activeSounds, [player](const auto &a, const auto &b) {
            return player->GetPosition().GetDistance(a.GetPosition())
                 < player->GetPosition().GetDistance(b.GetPosition());
        });

        if (maxSoundMarkers != -1 && activeSounds.size() > static_cast<size_t>(maxSoundMarkers)) {
            activeSounds.resize(maxSoundMarkers);
        }

        for (const auto &sound : activeSounds) {
            ProcessSoundMarker(sound.GetPosition(),
                               player->GetPosition(),
                               drawList,
                               camera->pos,
                               sound.name,
                               sound.soundEffect,
                               sound.maxDistance);
        }
    }

    // Delete later (Not Used)
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
                                        const std::string  &soundEffect,
                                        float               maxDistance) {
        if (!drawList) {
            return;
        }

        const float distance = cameraPos.GetDistance(soundPos);
        if (distanceFilterEnabled && distance > maxRenderDistance) {
            return;
        }

        const bool isObstructed =
            obstructionEffectEnabled
            && RenderObject::IsObjectObstructed(soundPos, maxDistance, 16, obstructionThreshold);

        const ImU32 markerColorConv =
            isObstructed ? IM_COL32(128, 128, 128, 255) : ConvertColor(markerColor);
        const ImU32 radiusIndicatorColorConv =
            isObstructed ? IM_COL32(64, 64, 64, 128) : ConvertColor(radiusIndicatorColor);
        const ImU32 tracerColorConv =
            isObstructed ? IM_COL32(64, 64, 64, 128) : ConvertColor(tracerColor);
        const ImU32 textColorConv =
            isObstructed ? IM_COL32(192, 192, 192, 255) : ConvertColor(textHoverColor);

        if (tracers) {
            RenderObject::DrawTracerLine(soundPos, playerPos, drawList, tracerColorConv, 2.0f);
        }

        DrawSoundMarker(soundPos,
                        playerPos,
                        distance,
                        drawList,
                        markerColorConv,
                        radiusIndicatorColorConv,
                        textColorConv,
                        name,
                        soundEffect,
                        maxDistance,
                        isObstructed);
    }

    void
        SoundMarker::DrawSoundMarker(const RE::NiPoint3 &soundPos,
                                     const RE::NiPoint3 &playerPos,
                                     float               distance,
                                     ImDrawList         *drawList,
                                     ImU32               localMarkerColor,
                                     ImU32               localRadiusIndicatorColor,
                                     ImU32               localTextColor,
                                     const std::string  &name,
                                     const std::string  &soundEffect,
                                     float               maxDistance,
                                     bool                isObstructed) {
        if (!drawList) {
            return;
        }

        const float markerSize = CalculateMarkerSize(distance);

        if (radiusIndicator) {
            ImU32 outlineColor = (enableRadiusOutlineColorChange
                                  && IsPlayerInSoundRadius(soundPos, playerPos, maxDistance))
                                   ? ConvertColor(radiusOutlineColor)
                                   : IM_COL32(0, 0, 0, 255);

            RenderObject::Draw3DCircleOutline(soundPos,
                                              maxDistance,
                                              drawList,
                                              outlineColor,
                                              radiusOutlineThickness,
                                              numSegmentsCircle);
            RenderObject::Draw3DCircle(
                soundPos, maxDistance, drawList, localRadiusIndicatorColor, numSegmentsCircle);
        }

        RenderObject::Draw3DSphere(
            soundPos, markerSize, drawList, localMarkerColor, numSegmentsSphere);

        if (textHover) {
            RenderObject::DrawTextAboveSphere(soundPos,
                                              markerSize,
                                              localTextColor,
                                              drawList,
                                              name,
                                              soundEffect,
                                              obstructionEffectEnabled && isObstructed);
        }
    }

    bool
        SoundMarker::IsPlayerInSoundRadius(const RE::NiPoint3 &soundPos,
                                           const RE::NiPoint3 &playerPos,
                                           float               maxDistance) {
        return soundPos.GetDistance(playerPos) <= maxDistance;
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

    void
        SoundMarker::SetMarkerColor(const ImVec4 color) {
        markerColor = color;
    }

    ImVec4
        SoundMarker::GetMarkerColor() {
        return markerColor;
    }

    void
        SoundMarker::SetRadiusIndicatorColor(const ImVec4 color) {
        radiusIndicatorColor = color;
    }

    ImVec4
        SoundMarker::GetRadiusIndicatorColor() {
        return radiusIndicatorColor;
    }

    void
        SoundMarker::SetTracerColor(const ImVec4 color) {
        tracerColor = color;
    }

    ImVec4
        SoundMarker::GetTracerColor() {
        return tracerColor;
    }

    void
        SoundMarker::SetTextHoverColor(const ImVec4 color) {
        textHoverColor = color;
    }

    ImVec4
        SoundMarker::GetTextHoverColor() {
        return textHoverColor;
    }

    void
        SoundMarker::SetEnableRadiusColorChange(bool enable) {
        enableRadiusOutlineColorChange = enable;
    }

    bool
        SoundMarker::IsEnableRadiusColorChange() {
        return enableRadiusOutlineColorChange;
    }

    void
        SoundMarker::SetRadiusOutlineColor(const ImVec4 color) {
        radiusOutlineColor = color;
    }

    ImVec4
        SoundMarker::GetRadiusOutlineColor() {
        return radiusOutlineColor;
    }
}
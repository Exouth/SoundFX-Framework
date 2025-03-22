#pragma once

#include <imgui.h>

namespace SoundFX {
    class SoundMarker {
      public:
        static void
            Render(ImDrawList *drawList);
        static void
            ToggleVisibility(std::optional<bool> forceState);
        static bool
            IsVisible();
        static void
            SetMaxRenderDistance(float distance);
        static void
            EnableDistanceFilter(bool enable);
        static bool
            IsDistanceFilterEnabled();
        static float
            GetMaxRenderDistance();
        static void
            EnableObstructionEffect(bool enable);
        static bool
            IsObstructionEffectEnabled();
        static void
            SetObstructionThreshold(float threshold);
        static float
            GetObstructionThreshold();
        static void
            SetRadiusIndicator(bool enable);
        static bool
            IsRadiusIndicatorEnabled();
        static void
            SetRadiusOutlineThickness(float thickness);
        static float
            GetRadiusOutlineThickness();
        static void
            EnableTracers(bool enable);
        static bool
            IsTracersEnabled();
        static void
            SetMaxSoundMarkers(int maxMarkers);
        static void
            SetNumSegmentsCircle(int numSegments);
        static int
            GetNumSegmentsCircle();
        static void
            SetNumSegmentsSphere(int numSegments);
        static int
            GetNumSegmentsSphere();
        static void
            EnableTextHover(bool enable);
        static bool
            IsTextHoverEnabled();
        static void
            SetMarkerColor(ImVec4 color);
        static ImVec4
            GetMarkerColor();
        static void
            SetTracerColor(ImVec4 color);
        static ImVec4
            GetTracerColor();
        static void
            SetRadiusIndicatorColor(ImVec4 color);
        static ImVec4
            GetRadiusIndicatorColor();
        static void
            SetTextHoverColor(ImVec4 color);
        static ImVec4
            GetTextHoverColor();
        static void
            SetEnableRadiusColorChange(bool enable);
        static bool
            IsEnableRadiusColorChange();
        static void
            SetRadiusOutlineColor(ImVec4 color);
        static ImVec4
            GetRadiusOutlineColor();

      private:
        static std::vector<std::tuple<RE::NiPoint3, std::string, std::string>>
            GetActiveSoundPositions();
        static void
            ProcessSoundMarker(const RE::NiPoint3 &soundPos,
                               const RE::NiPoint3 &playerPos,
                               ImDrawList         *drawList,
                               const std::string  &name,
                               const std::string  &eventType,
                               const std::string  &soundEffect,
                               float               maxDistance);
        static void
            DrawSoundMarker(const RE::NiPoint3 &soundPos,
                            const RE::NiPoint3 &playerPos,
                            float               distance,
                            ImDrawList         *drawList,
                            ImU32               localMarkerColor,
                            ImU32               localRadiusIndicatorColor,
                            ImU32               localTextColor,
                            const std::string  &name,
                            const std::string  &eventType,
                            const std::string  &soundEffect,
                            float               maxDistance,
                            bool                isObstructed);

        static bool
            IsPlayerInSoundRadius(const RE::NiPoint3 &soundPos,
                                  const RE::NiPoint3 &playerPos,
                                  float               maxDistance);

        static bool   showSoundMarkers;
        static bool   distanceFilterEnabled;
        static bool   obstructionEffectEnabled;
        static float  maxRenderDistance;
        static float  obstructionThreshold;
        static bool   radiusIndicator;
        static float  radiusOutlineThickness;
        static bool   tracers;
        static int    maxSoundMarkers;
        static int    numSegmentsCircle;
        static int    numSegmentsSphere;
        static bool   textHover;
        static ImVec4 markerColor;
        static ImVec4 radiusIndicatorColor;
        static ImVec4 tracerColor;
        static ImVec4 textHoverColor;
        static bool   enableRadiusOutlineColorChange;
        static ImVec4 radiusOutlineColor;
    };
}
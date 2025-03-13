#pragma once

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
            SetSoundRadius(float radius);
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

      private:
        static std::vector<RE::NiPoint3>
            GetActiveSoundPositions();
        static void
            ProcessSoundMarker(const RE::NiPoint3 &soundPos,
                               const RE::NiPoint3 &playerPos,
                               ImDrawList         *drawList,
                               const RE::NiPoint3 &cameraPos);
        static void
            DrawSoundMarker(const RE::NiPoint3 &soundPos,
                            float               distance,
                            ImDrawList         *drawList,
                            ImU32               markerColor,
                            ImU32               tracerColor);

        static bool  showSoundMarkers;
        static bool  distanceFilterEnabled;
        static bool  obstructionEffectEnabled;
        static float maxRenderDistance;
        static float soundRadius;
        static float obstructionThreshold;
        static bool  radiusIndicator;
        static float radiusOutlineThickness;
        static bool  tracers;
    };
}
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

      private:
        static bool  showSoundMarkers;
        static bool  distanceFilterEnabled;
        static bool  obstructionEffectEnabled;
        static float maxRenderDistance;
        static float soundRadius;
        static float obstructionThreshold;
    };
}
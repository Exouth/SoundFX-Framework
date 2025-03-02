#pragma once

namespace SoundFX {
    class SoundMarker {
      public:
        static void
            Render();
        static void
            ToggleVisibility();
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

      private:
        static bool  showSoundMarkers;
        static bool  distanceFilterEnabled;
        static float maxRenderDistance;
        static bool
            WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float &depth);
    };
}
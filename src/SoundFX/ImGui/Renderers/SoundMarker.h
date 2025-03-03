#pragma once

namespace SoundFX {
    class SoundMarker {
      public:
        static void
            Render(ImDrawList *drawList);
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
        static void
            SetSoundRadius(float radius);

      private:
        static bool  showSoundMarkers;
        static bool  distanceFilterEnabled;
        static float maxRenderDistance;
        static float soundRadius;
    };
}
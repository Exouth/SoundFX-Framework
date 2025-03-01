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

      private:
        static bool showSoundMarkers;
        static bool
            WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float &depth);
    };
}
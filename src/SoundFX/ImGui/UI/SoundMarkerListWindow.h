#pragma once

namespace SoundFX {
    class SoundMarkerListWindow {
      public:
        static void
            Render();

      private:
        static void
            RenderActiveSoundCount(size_t count);
    };
}
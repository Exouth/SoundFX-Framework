#pragma once

constexpr auto ICON_FA_ELLIPSIS = "\xef\x85\x81";  // Unicode: f141 / fa-ellipsis

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
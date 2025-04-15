#pragma once

namespace SoundFX {
    class Theme {
      public:
        static void
            ApplyTheme();
        static void
            RenderFooterLine();
        static void
            DrawVerticalSeparator(float height = 20.0f);
    };
}
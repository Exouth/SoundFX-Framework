#pragma once

namespace SoundFX {
    class ImGuiManager {
      public:
        static void
            Initialize();
        static void
            Render();
        static void
            Shutdown();
        static void
            ToggleUI();
        static bool
            IsUIVisible();

      private:
        static bool showDebugUI;
    };
}
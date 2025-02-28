#pragma once

namespace SoundFX {
    class ImGuiManager {
      public:
        static void
            Initialize(HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *context);
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
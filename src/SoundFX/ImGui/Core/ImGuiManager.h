#pragma once

#include <Windows.h>
#include <imgui_impl_dx11.h>

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
        static void
            UpdateInputState();

        static bool showSoundMarkerList;

      private:
        static void
            RenderBackground();
        static void
                    RenderForeground();
        static bool showDebugUI;
        static int  debugUIKey;
    };
}
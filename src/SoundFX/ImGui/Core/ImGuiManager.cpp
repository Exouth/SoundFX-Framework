#include "ImGuiManager.h"
#include "ImGui/UI/MainWindow.h"

namespace SoundFX {
    bool ImGuiManager::showDebugUI = false;

    void
        ImGuiManager::Initialize(HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *context) {
        ImGui::CreateContext();
        ImGui_ImplDX11_Init(device, context);
        ImGui_ImplWin32_Init(hwnd);
    }

    void
        ImGuiManager::Render() {
        if (GetAsyncKeyState(VK_F1) & 1) {
            ToggleUI();
        }

        ImGuiIO &io        = ImGui::GetIO();
        io.MouseDrawCursor = showDebugUI;

        if (!showDebugUI)
            return;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        MainWindow::Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void
        ImGuiManager::Shutdown() {

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void
        ImGuiManager::ToggleUI() {
        showDebugUI = !showDebugUI;
        spdlog::debug("ImGui UI {}", showDebugUI ? "activated" : "disabled");
    }

    bool
        ImGuiManager::IsUIVisible() {
        return showDebugUI;
    }
}
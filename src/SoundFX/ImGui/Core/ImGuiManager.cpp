#include "ImGuiManager.h"
#include "ImGui/Renderers/SoundMarker.h"
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

        if (auto ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) {
            return;
        }

        ImGuiIO &io        = ImGui::GetIO();
        io.MouseDrawCursor = showDebugUI;

        UpdateInputState();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (showDebugUI) {
            MainWindow::Render();
        }

        SoundMarker::Render();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void
        ImGuiManager::UpdateInputState() {
        auto *controlMap = RE::ControlMap::GetSingleton();
        if (!controlMap) {
            return;
        }

        ImGuiIO &io = ImGui::GetIO();

        if (showDebugUI) {
            io.WantCaptureMouse             = true;
            io.WantCaptureKeyboard          = true;
            controlMap->ignoreKeyboardMouse = true;
        } else {
            io.WantCaptureMouse             = false;
            io.WantCaptureKeyboard          = false;
            controlMap->ignoreKeyboardMouse = false;
        }
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
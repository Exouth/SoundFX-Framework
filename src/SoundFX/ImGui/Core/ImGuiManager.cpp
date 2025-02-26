#include "ImGuiManager.h"
#include "ImGui/UI/MainWindow.h"

namespace SoundFX {
    bool ImGuiManager::showDebugUI = false;

    HWND
        GetSkyrimWindow() {
        auto rendererWindow = RE::BSGraphics::Renderer::GetCurrentRenderWindow();
        return rendererWindow ? reinterpret_cast<HWND>(rendererWindow->hWnd) : nullptr;
    }

    void
        ImGuiManager::Initialize() {
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        HWND hwnd = GetSkyrimWindow();
        if (!hwnd) {
            spdlog::error("Couldnt find Skyrim windows!");
            return;
        }

        if (!ImGui_ImplWin32_Init(hwnd)) {
            spdlog::error("ImGui_ImplWin32_Init failed!");
            return;
        }
        if (!ImGui_ImplDX11_Init(
                reinterpret_cast<ID3D11Device *>(
                    RE::BSGraphics::Renderer::GetSingleton()->GetDevice()),
                reinterpret_cast<ID3D11DeviceContext *>(
                    RE::BSGraphics::Renderer::GetSingleton()->GetRendererData()->context))) {
            spdlog::error("ImGui_ImplDX11_Init failed!");
            return;
        }

        spdlog::info("ImGui initialized successfully!");
    }

    void
        ImGuiManager::Render() {
        if (GetAsyncKeyState(VK_F1) & 1) {
            ToggleUI();
        }

        ImGuiIO &io        = ImGui::GetIO();
        io.MouseDrawCursor = showDebugUI;

        if (showDebugUI) {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            io.WantCaptureMouse                                 = true;
            io.WantCaptureKeyboard                              = true;
            RE::ControlMap::GetSingleton()->ignoreKeyboardMouse = true;
        } else {
            io.WantCaptureMouse                                 = false;
            io.WantCaptureKeyboard                              = false;
            RE::ControlMap::GetSingleton()->ignoreKeyboardMouse = false;
        }

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
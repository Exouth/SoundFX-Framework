#include "ImGuiManager.h"
#include "Config/ConfigManager.h"
#include "Font_Awesome_6_Free_Solid_900_Edited.otf.h"
#include "ImGui/Renderers/SoundMarker.h"
#include "ImGui/Styles/Theme.h"
#include "ImGui/UI/LogViewer.h"
#include "ImGui/UI/MainWindow.h"
#include "ImGui/UI/SoundMarkerListWindow.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>

namespace SoundFX {
    bool        ImGuiManager::showDebugUI         = false;
    bool        ImGuiManager::showSoundMarkerList = true;
    int         ImGuiManager::debugUIKey          = VK_F1;
    std::string ImGuiManager::lastFocusedWindow   = "SoundFX-Framework Debug";
    bool        ImGuiManager::justActivatedUI     = false;

    void
        ImGuiManager::Initialize(HWND hwnd, ID3D11Device *device, ID3D11DeviceContext *context) {
        ImGui::CreateContext();
        ImGui_ImplDX11_Init(device, context);
        ImGui_ImplWin32_Init(hwnd);

        Theme::ApplyTheme();

        debugUIKey = Config::ConfigManager::GetInstance().GetValue<int>(
            "GeneralSettings", "DebugUIKey", VK_F1);

        LogViewer::Initialize();

        ImGuiIO &io = ImGui::GetIO();
        io.Fonts->AddFontDefault();
        static constexpr ImWchar icons_ranges[] = {0xf000, 0xf3ff, 0};
        ImFontConfig             icons_config;
        icons_config.MergeMode     = true;
        icons_config.PixelSnapH    = true;
        icons_config.GlyphOffset.y = 3.0f;
        io.Fonts->AddFontFromMemoryTTF(FontAwesome6Free_Solid_Edited_otf,
                                       sizeof(FontAwesome6Free_Solid_Edited_otf),
                                       16.0f,
                                       &icons_config,
                                       icons_ranges);
        io.Fonts->Build();
    }

    void
        ImGuiManager::Render() {
        if (GetAsyncKeyState(debugUIKey) & 1) {
            ToggleUI();
        }

        if (const auto ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) {
            return;
        }

        ImGuiIO &io        = ImGui::GetIO();
        io.MouseDrawCursor = showDebugUI;

        UpdateInputState();

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RenderBackground();

        if (showDebugUI) {
            MainWindow::Render();
            if (showSoundMarkerList) {
                SoundMarkerListWindow::Render();
            }
            LogViewer::Render();
        }

        if (justActivatedUI) {
            ImGui::SetWindowFocus(lastFocusedWindow.c_str());
            justActivatedUI = false;
        }

        RenderForeground();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void
        ImGuiManager::RenderBackground() {
        ImDrawList *backgroundDrawList = ImGui::GetBackgroundDrawList();
        SoundMarker::Render(backgroundDrawList);
    }

    void
        ImGuiManager::RenderForeground() {
        // ImDrawList *foregroundDrawList = ImGui::GetForegroundDrawList();
        // For Later maybe if there comes more
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

        LogViewer::Shutdown();
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void
        ImGuiManager::ToggleUI() {
        showDebugUI = !showDebugUI;

        if (showDebugUI) {
            showSoundMarkerList = true;
            justActivatedUI     = true;
        }

        spdlog::debug("ImGui UI {}", showDebugUI ? "activated" : "disabled");
    }

    bool
        ImGuiManager::IsUIVisible() {
        return showDebugUI;
    }

    void
        ImGuiManager::SetLastFocusedWindow(const std::string &name) {
        lastFocusedWindow = name;
    }

    void
        ImGuiManager::TrackFocusOnClick() {
        if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows)
            && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            if (const auto *window = ImGui::GetCurrentWindow(); window && window->Name) {
                SetLastFocusedWindow(window->Name);
            }
        }
    }
}
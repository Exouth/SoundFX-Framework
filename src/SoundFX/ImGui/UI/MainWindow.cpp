#include "MainWindow.h"
#include "ImGui/Core/ImGuiManager.h"
#include "LogViewer.h"
#include "SettingsUI.h"

namespace SoundFX {

    void
        MainWindow::Render() {
        bool open = true;

        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("SoundFX-Framework Debug", &open);

        ImGui::SeparatorText("Settings");
        ImGui::Spacing();

        SettingsUI::RenderSettingsUI();
        ImGui::Spacing();

        ImGui::SeparatorText("Extra");

        ImGui::Spacing();
        if (ImGui::Button("Open LogView") && !LogViewer::IsVisible()) {
            LogViewer::ToggleVisibility();
        }

        ImGui::End();

        if (!open) {
            ImGuiManager::ToggleUI();
        }
    }
}
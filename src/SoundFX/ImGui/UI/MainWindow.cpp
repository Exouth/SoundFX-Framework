#include "MainWindow.h"
#include "ImGui/Core/ImGuiManager.h"

namespace SoundFX {
    void
        MainWindow::Render() {

        bool open = true;

        ImGui::Begin("SoundFX-Framework Debug", &open);

        ImGui::Text("Debug UI");
        if (ImGui::Button("Close UI")) {
            ImGuiManager::ToggleUI();
        }
        ImGui::End();

        if (!open) {
            ImGuiManager::ToggleUI();
        }
    }
}
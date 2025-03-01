#include "MainWindow.h"
#include "ImGui/Core/ImGuiManager.h"
#include "ImGui/Renderers/SoundMarker.h"

namespace SoundFX {

    void
        MainWindow::Render() {
        bool open = true;

        ImGui::Begin("SoundFX-Framework Debug", &open);

        ImGui::Text("Debug UI");
        if (ImGui::Button("Close UI")) {
            ImGuiManager::ToggleUI();
        }

        bool showSoundMarkers = SoundMarker::IsVisible();
        if (ImGui::Checkbox("Show Sound Markers", &showSoundMarkers)) {
            SoundMarker::ToggleVisibility();
        }

        ImGui::End();

        if (!open) {
            ImGuiManager::ToggleUI();
        }
    }
}
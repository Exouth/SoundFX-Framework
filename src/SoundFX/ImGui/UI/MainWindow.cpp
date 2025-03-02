#include "MainWindow.h"
#include "ImGui/Core/ImGuiManager.h"
#include "ImGui/Renderers/SoundMarker.h"

namespace SoundFX {

    void
        MainWindow::Render() {
        bool open = true;

        ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));

        ImGui::Begin("SoundFX-Framework Debug", &open);

        ImGui::Text("Debug UI");
        if (ImGui::Button("Close UI")) {
            ImGuiManager::ToggleUI();
        }

        bool showSoundMarkers = SoundMarker::IsVisible();
        if (ImGui::Checkbox("Show Sound Markers", &showSoundMarkers)) {
            SoundMarker::ToggleVisibility();
        }

        RenderSettingsUI();

        ImGui::End();

        if (!open) {
            ImGuiManager::ToggleUI();
        }
    }

    void
        MainWindow::RenderSettingsUI() {
        static bool distanceFilterEnabled = SoundMarker::IsDistanceFilterEnabled();
        if (ImGui::Checkbox("Enable Distance Filter", &distanceFilterEnabled)) {
            SoundMarker::EnableDistanceFilter(distanceFilterEnabled);
        }

        static float maxRenderDistance = SoundMarker::GetMaxRenderDistance();
        if (ImGui::SliderFloat("Max Render Distance", &maxRenderDistance, 0.0f, 10000.0f)) {
            SoundMarker::SetMaxRenderDistance(maxRenderDistance);
        }
    }
}
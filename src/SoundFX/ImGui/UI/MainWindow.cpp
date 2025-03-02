#include "MainWindow.h"
#include "ImGui/Core/ImGuiManager.h"
#include "ImGui/Renderers/SoundMarker.h"

#define ICON_FA_UNDO "\xef\x8b\xaa"  // Unicode: f2ea / fa-rotate-left

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
        static bool           distanceFilterEnabled        = SoundMarker::IsDistanceFilterEnabled();
        static constexpr bool defaultDistanceFilterEnabled = true;

        if (ImGui::Checkbox("Enable Distance Filter", &distanceFilterEnabled)) {
            SoundMarker::EnableDistanceFilter(distanceFilterEnabled);
        }
        if (distanceFilterEnabled != defaultDistanceFilterEnabled) {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##DistanceFilter")) {
                distanceFilterEnabled = defaultDistanceFilterEnabled;
                SoundMarker::EnableDistanceFilter(distanceFilterEnabled);
            }
        }

        static float           maxRenderDistance        = SoundMarker::GetMaxRenderDistance();
        static constexpr float defaultMaxRenderDistance = 4000.0f;

        if (ImGui::SliderFloat("Max Render Distance", &maxRenderDistance, 0.0f, 10000.0f)) {
            SoundMarker::SetMaxRenderDistance(maxRenderDistance);
        }
        if (constexpr float epsilon = 0.01f;
            std::fabs(maxRenderDistance - defaultMaxRenderDistance) > epsilon) {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_UNDO "##MaxRenderDistance")) {
                maxRenderDistance = defaultMaxRenderDistance;
                SoundMarker::SetMaxRenderDistance(maxRenderDistance);
            }
        }
    }
}
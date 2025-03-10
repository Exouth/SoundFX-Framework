#include "SoundMarker.h"
#include "RE/P/PlayerCamera.h"

namespace SoundFX {
    bool SoundMarker::showSoundMarkers = false;

    bool
        SoundMarker::WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos) {
        const auto *camera = RE::PlayerCamera::GetSingleton();
        if (!camera) {
            return false;
        }

        const auto root = camera->cameraRoot.get();
        if (!root || root->GetChildren().empty()) {
            return false;
        }

        if (std::strcmp(root->GetChildren()[0]->GetRTTI()->name, "NiCamera") == 0) {
            const auto niCamera = static_cast<RE::NiCamera *>(root->GetChildren()[0].get());

            const auto &runtimeData = niCamera->GetRuntimeData();
            const auto &viewport    = niCamera->GetRuntimeData2().port;

            float      x, y, z;
            const bool visible = RE::NiCamera::WorldPtToScreenPt3(
                runtimeData.worldToCam, viewport, worldPos, x, y, z, 0.0001f);

            if (!visible || z <= 0.0f) {
                return false;  // Is Behind Camera
            }

            const ImGuiIO &io = ImGui::GetIO();
            screenPos.x       = x * io.DisplaySize.x;
            screenPos.y       = (1.0f - y) * io.DisplaySize.y;

            return true;
        }

        return false;
    }

    void
        SoundMarker::Render() {
        if (!showSoundMarkers)
            return;

        ImDrawList *drawList = ImGui::GetForegroundDrawList();

        // Draw the Markers (For Later)
        /*std::vector<RE::NiPoint3> testPositions = {{205.0f, 200.0f, 200.0f},
                                                   {210.0f, 205.0f, 200.0f}};
        for (const auto &pos : testPositions) {
            if (ImVec2 screenPos; WorldToScreen(pos, screenPos)) {
                drawList->AddCircleFilled(screenPos, 10.0f, IM_COL32(255, 255, 0, 255));
            }
        }*/
    }

    void
        SoundMarker::ToggleVisibility() {
        showSoundMarkers = !showSoundMarkers;
        spdlog::debug("Sound markers visibility toggled: {}", showSoundMarkers);
    }

    bool
        SoundMarker::IsVisible() {
        return showSoundMarkers;
    }
}
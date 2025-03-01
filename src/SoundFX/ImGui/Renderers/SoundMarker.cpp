#include "SoundMarker.h"
#include "RE/P/PlayerCamera.h"

namespace SoundFX {
    bool SoundMarker::showSoundMarkers = false;

    bool
        SoundMarker::WorldToScreen(const RE::NiPoint3 &worldPos, ImVec2 &screenPos, float &depth) {
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

            const auto &[worldToCam] = niCamera->GetRuntimeData();
            const auto &viewport     = niCamera->GetRuntimeData2().port;

            float      x, y, z;
            const bool visible =
                RE::NiCamera::WorldPtToScreenPt3(worldToCam, viewport, worldPos, x, y, z, 0.0001f);

            if (!visible || z <= 0.0f) {
                return false;  // Is Behind Camera
            }

            const ImGuiIO &io = ImGui::GetIO();
            screenPos.x       = x * io.DisplaySize.x;
            screenPos.y       = (1.0f - y) * io.DisplaySize.y;
            depth             = z;

            return true;
        }

        return false;
    }

    void
        SoundMarker::Render() {
        if (!showSoundMarkers)
            return;

        const auto *camera = RE::PlayerCamera::GetSingleton();
        if (!camera) {
            return;
        }

        ImDrawList *drawList = ImGui::GetForegroundDrawList();

        /*const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }
        // Draw the Markers (For Later)
        const std::vector<RE::NiPoint3> testPositions = {
            {player->GetPosition()}, {7729.8643f, -68789.37f, 4515.15f}, {300.0f, 300.0f, 300.0f}};

        for (const auto &pos : testPositions) {
            ImVec2 screenPos;
            float  depth;

            if (WorldToScreen(pos, screenPos, depth)) {
                const float distance = camera->pos.GetDistance(pos);
                const float size     = CalculateMarkerSize(distance);

                drawList->AddCircleFilled(screenPos, size, IM_COL32(255, 255, 0, 255));
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
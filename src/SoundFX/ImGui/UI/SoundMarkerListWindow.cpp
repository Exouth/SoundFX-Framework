#include "SoundMarkerListWindow.h"
#include "ImGui/Core/ImGuiManager.h"

namespace SoundFX {

    void
        SoundMarkerListWindow::Render() {
        if (!ImGuiManager::showSoundMarkerList) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Active Sound Markers", &ImGuiManager::showSoundMarkerList)) {
            ImGui::End();
            return;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            ImGui::Text("No player found.");
            ImGui::End();
            return;
        }

        auto activeSounds = SoundManager::GetSortedActiveSounds();

        static int sortMode = 0;
        ImGui::Text("Sort by: ");
        ImGui::SameLine();
        if (ImGui::RadioButton("Distance", sortMode == 0)) {
            sortMode = 0;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Name", sortMode == 1)) {
            sortMode = 1;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Event Type", sortMode == 2)) {
            sortMode = 2;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Sound Effect", sortMode == 3)) {
            sortMode = 3;
        }

        switch (sortMode) {
        case 1:
            std::ranges::sort(activeSounds, [](const auto &a, const auto &b) {
                return NaturalStringCompare(a.name, b.name);
            });
            break;
        case 2:
            std::ranges::sort(activeSounds, [](const auto &a, const auto &b) {
                return NaturalStringCompare(a.eventType, b.eventType);
            });
            break;
        case 3:
            std::ranges::sort(activeSounds, [](const auto &a, const auto &b) {
                return NaturalStringCompare(a.soundEffect, b.soundEffect);
            });
            break;
        default: break;
        }

        if (activeSounds.empty()) {
            ImGui::Text("No active sound markers.");
            ImGui::End();
            return;
        }

        ImGui::Separator();

        if (ImGui::BeginTable(
                "SoundMarkersTable", 8, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("EventType", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Effect", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Position (X, Y, Z)", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Max Distance", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Ref. Distance", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Distance to Player", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("3D", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableHeadersRow();

            for (const auto &sound : activeSounds) {
                RE::NiPoint3 pos              = sound.GetPosition();
                const float  distanceToPlayer = player->GetPosition().GetDistance(pos);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("%s", sound.name.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", sound.eventType.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", sound.soundEffect.c_str());
                ImGui::TableNextColumn();
                ImGui::Text("(%.1f, %.1f, %.1f)", pos.x, pos.y, pos.z);
                ImGui::TableNextColumn();
                ImGui::Text("%.1f", sound.maxDistance);
                ImGui::TableNextColumn();
                ImGui::Text("%.1f", sound.referenceDistance);
                ImGui::TableNextColumn();
                ImGui::Text("%.1f", distanceToPlayer);
                ImGui::TableNextColumn();
                ImGui::Text("%s", sound.is3D ? "Yes" : "No");
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

}
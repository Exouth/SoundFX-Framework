#include "CellEventHandler.h"
#include "Sound/SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    void
        CellEventHandler::SetupCellTasks() {
        StartCellTask([this]() { ProcessCellEnterTask(); }, true);
        StartCellTask([this]() { ProcessAmbienceSoundTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    void
        CellEventHandler::ProcessCellEnterTask() const {

        static RE::TESObjectCELL *lastCell = nullptr;

        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        if (player->GetWorldspace()) {
            lastCell = nullptr;
            return;
        }

        auto *currentCell = player->GetParentCell();
        if (!currentCell) {
            return;
        }

        auto *menuManager = RE::UI::GetSingleton();
        if (!menuManager) {
            return;
        }

        if (!menuManager->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && currentCell != lastCell) {
            lastCell = currentCell;

            const auto &cells = jsonLoader.getItems("cells");
            for (const auto &cellEvents : cells | std::views::values) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(cellEvents.editorID, cellEvents.pluginName);

                if (resolvedFormID == currentCell->formID) {
                    for (const auto &jsonEvent : cellEvents.events) {
                        if (jsonEvent.type == "Enter") {
                            const float randomValue = GenerateRandomFloat();
                            if (randomValue <= jsonEvent.chance) {
                                PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                            }
                            return;
                        }
                    }
                }
            }
        }
    }

    // Further completion will be made when 3D sound is better implemented
    void
        CellEventHandler::ProcessAmbienceSoundTask() const {

        const auto player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        const auto *currentCell = player->GetParentCell();
        if (!currentCell) {
            return;
        }

        auto *menuManager = RE::UI::GetSingleton();
        if (!menuManager) {
            return;
        }

        if (!menuManager->IsMenuOpen(RE::LoadingMenu::MENU_NAME)) {

            const auto &cells = jsonLoader.getItems("cells");
            for (const auto &cellEvents : cells | std::views::values) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(cellEvents.editorID, cellEvents.pluginName);

                if (resolvedFormID == currentCell->formID) {
                    for (const auto &jsonEvent : cellEvents.events) {
                        if (jsonEvent.type == "Ambience") {
                            const float randomValue = GenerateRandomFloat();
                            if (randomValue <= jsonEvent.chance) {
                                PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                            }
                            return;
                        }
                    }
                }
            }
        }
    }
}
#include "CellEventHandler.h"
#include "Sound/SoundManager.h"
#include "Utility.h"

namespace SoundFX {

    void
        CellEventHandler::SetupCellTasks() {
        StartCellTask([this] { ProcessCellEnterTask(); }, true);
        StartCellTask([this] { ProcessAmbienceSoundTask(); }, true);
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

            for (const auto &cells = jsonLoader->getItems("cells");
                 const auto &cellEvents : cells | std::views::values) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(cellEvents.editorID, cellEvents.pluginName);

                if (resolvedFormID == currentCell->formID) {
                    for (const auto &jsonEvent : cellEvents.events) {
                        if (jsonEvent.type == "Enter") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(cellEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        {8029.8643f, -69089.37f, 4815.15f},
                                                        1000.0f,
                                                        jsonEvent.volume,
                                                        jsonEvent.isAbsoluteVolume,
                                                        true);
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

            for (const auto &cells = jsonLoader->getItems("cells");
                 const auto &cellEvents : cells | std::views::values) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(cellEvents.editorID, cellEvents.pluginName);

                if (resolvedFormID == currentCell->formID) {
                    for (const auto &jsonEvent : cellEvents.events) {
                        if (jsonEvent.type == "Ambience") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(cellEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        {8029.8643f, -69089.37f, 4815.15f},
                                                        1000.0f,
                                                        jsonEvent.volume,
                                                        jsonEvent.isAbsoluteVolume,
                                                        true);
                            }
                            return;
                        }
                    }
                }
            }
        }
    }
}
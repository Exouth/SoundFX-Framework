#include "CellEventHandler.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    void
        CellEventHandler::SetupCellTasks() {
        StartCellTask([this]() { ProcessCellEnterTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    void
        CellEventHandler::ProcessCellEnterTask() {

        static RE::TESObjectCELL *lastCell = nullptr;

        auto player = RE::PlayerCharacter::GetSingleton();
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
            for (const auto &[cellName, cellEvents] : cells) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(cellEvents.editorID, cellEvents.pluginName);

                if (resolvedFormID == currentCell->formID) {
                    for (const auto &jsonEvent : cellEvents.events) {
                        if (jsonEvent.type == "Enter") {
                            float randomValue = static_cast<float>(rand()) / RAND_MAX;
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
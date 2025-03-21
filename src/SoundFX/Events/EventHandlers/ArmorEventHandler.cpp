#include "ArmorEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                        RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessEvent(const RE::TESEquipEvent *event,
                                        RE::BSTEventSource<RE::TESEquipEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessEquipEvent(event), ProcessUnequipEvent(event)});
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &armors = jsonLoader->getItems("armors");
             const auto &armorEvents : armors | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(armorEvents.editorID, armorEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : armorEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessEquipEvent(const RE::TESEquipEvent *event) const {

        if (event == nullptr || event->baseObject == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObject);
        if (!item) {
            spdlog::warn("Failed to lookup item for EquipEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!event->equipped) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->actor->GetObjectReference()
            == RE::PlayerCharacter::GetSingleton()->GetObjectReference()) {
            for (const auto &armors = jsonLoader->getItems("armors");
                 const auto &armorEvents : armors | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    armorEvents.editorID, armorEvents.pluginName);

                if (resolvedFormID == item->formID) {
                    for (const auto &jsonEvent : armorEvents.events) {
                        if (jsonEvent.type == "Equip") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                            }
                            return RE::BSEventNotifyControl::kContinue;
                        }
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessUnequipEvent(const RE::TESEquipEvent *event) const {

        if (event == nullptr || event->baseObject == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObject);
        if (!item) {
            spdlog::warn("Failed to lookup item for EquipEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->equipped) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->actor->GetObjectReference()
            == RE::PlayerCharacter::GetSingleton()->GetObjectReference()) {
            for (const auto &armors = jsonLoader->getItems("armors");
                 const auto &armorEvents : armors | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    armorEvents.editorID, armorEvents.pluginName);

                if (resolvedFormID == item->formID) {
                    for (const auto &jsonEvent : armorEvents.events) {
                        if (jsonEvent.type == "Unequip") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                            }
                            return RE::BSEventNotifyControl::kContinue;
                        }
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }

        return RE::BSEventNotifyControl::kContinue;
    }
}
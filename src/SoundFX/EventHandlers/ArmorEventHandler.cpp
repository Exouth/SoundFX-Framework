#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "ArmorEventHandler.h"
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
        return EventHandlerManager::ProcessMultipleEvents({ProcessEquipEvent(event), ProcessUnequipEvent(event)});
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &weapons = jsonLoader.getItems("armors");
        for (const auto &[itemName, itemEvents] : weapons) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : itemEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
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
        ArmorEventHandler::ProcessEquipEvent(const RE::TESEquipEvent *event) {

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
            const auto &weapons = jsonLoader.getItems("armors");
            for (const auto &[itemName, itemEvents] : weapons) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

                if (resolvedFormID == item->formID) {
                    for (const auto &jsonEvent : itemEvents.events) {
                        if (jsonEvent.type == "Equip") {
                            float randomValue = static_cast<float>(rand()) / RAND_MAX;
                            if (randomValue <= jsonEvent.chance) {
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
        ArmorEventHandler::ProcessUnequipEvent(const RE::TESEquipEvent *event) {

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
            const auto &weapons = jsonLoader.getItems("armors");
            for (const auto &[itemName, itemEvents] : weapons) {
                const auto resolvedFormID =
                    GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

                if (resolvedFormID == item->formID) {
                    for (const auto &jsonEvent : itemEvents.events) {
                        if (jsonEvent.type == "Unequip") {
                            float randomValue = static_cast<float>(rand()) / RAND_MAX;
                            if (randomValue <= jsonEvent.chance) {
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

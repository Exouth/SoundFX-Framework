#include "WeaponEventHandler.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESEquipEvent *event,
                                         RE::BSTEventSource<RE::TESEquipEvent> *) {
        return ProcessEquipEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESHitEvent *event,
                                         RE::BSTEventSource<RE::TESHitEvent> *) {
        return ProcessHitEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEquipEvent(const RE::TESEquipEvent *event) {

        if (event == nullptr || event->baseObject == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObject);
        if (!item) {
            spdlog::warn("Failed to lookup item for EquipEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &weapons = jsonLoader.getItems("weapons");
        for (const auto &[itemName, itemEvents] : weapons) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : itemEvents.events) {
                    if (jsonEvent.type == "Equip") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            spdlog::info("Playing Equip sound for weapon: {}",
                                         jsonEvent.soundEffect);
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
        WeaponEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &weapons = jsonLoader.getItems("weapons");
        for (const auto &[itemName, itemEvents] : weapons) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : itemEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            spdlog::info("Playing PickUp sound for weapon: {}",
                                         jsonEvent.soundEffect);
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
        WeaponEventHandler::ProcessHitEvent(const RE::TESHitEvent *event) {

        if (event == nullptr || event->target == 0 || event->source == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *target =
            RE::TESForm::LookupByID(event->target->formID);
        if (!target) {
            spdlog::warn("Failed to lookup target for HitEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &weapons = jsonLoader.getItems("weapons");
        for (const auto &[itemName, itemEvents] : weapons) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

            if (resolvedFormID
                == event->source) {
                for (const auto &jsonEvent : itemEvents.events) {
                    if (jsonEvent.type == "Hit") {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            spdlog::info("Playing Hit sound for weapon: {}", jsonEvent.soundEffect);
                            PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

}
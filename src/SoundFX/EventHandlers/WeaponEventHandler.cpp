#include "WeaponEventHandler.h"

#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    std::unordered_map<std::string, WeaponEventHandler::EventAction> WeaponEventHandler::actionMap;

    // InitAttackType temporary use for Filtering
    void
        WeaponEventHandler::InitializeAttackTypeHandlers() {
        actionMap["PowerAttack"] = [](EventVariant event, const std::string &soundEffect) {
            if (auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {
                if ((*hitEvent)->flags.any(RE::TESHitEvent::Flag::kPowerAttack)) {
                    PlayCustomSoundAsDescriptor(soundEffect);
                }
            }
        };
        actionMap["SneakAttack"] = [](EventVariant event, const std::string &soundEffect) {
            if (auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {
                if ((*hitEvent)->flags.any(RE::TESHitEvent::Flag::kSneakAttack)) {
                    PlayCustomSoundAsDescriptor(soundEffect);
                }
            }
        };
        actionMap["BashAttack"] = [](EventVariant event, const std::string &soundEffect) {
            if (auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {
                if ((*hitEvent)->flags.any(RE::TESHitEvent::Flag::kBashAttack)) {
                    PlayCustomSoundAsDescriptor(soundEffect);
                }
            }
        };
        actionMap["NormalAttack"] = [](EventVariant event, const std::string &soundEffect) {
            if (auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {

                const RE::TESForm *item = RE::TESForm::LookupByID((*hitEvent)->source);
                if (!item) {
                    return RE::BSEventNotifyControl::kContinue;
                }

                const auto *hit = item->As<RE::TESObjectWEAP>();
                if (!hit) {
                    return RE::BSEventNotifyControl::kContinue;
                }

                if (hit->GetAttackDamage() > 0
                    && !(*hitEvent)->flags.any(RE::TESHitEvent::Flag::kBashAttack,
                                               RE::TESHitEvent::Flag::kHitBlocked,
                                               RE::TESHitEvent::Flag::kPowerAttack,
                                               RE::TESHitEvent::Flag::kSneakAttack)) {
                    PlayCustomSoundAsDescriptor(soundEffect);
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        };
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESEquipEvent *event,
                                         RE::BSTEventSource<RE::TESEquipEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessEquipEvent(event), ProcessIdleEvent(event)});
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

        const auto &weapons = jsonLoader.getItems("weapons");
        for (const auto &[itemName, itemEvents] : weapons) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(itemEvents.editorID, itemEvents.pluginName);

            if (resolvedFormID == event->source) {
                for (const auto &jsonEvent : itemEvents.events) {
                    if (jsonEvent.type == "Hit") {
                        if (actionMap.find(jsonEvent.details.hitType.value()) != actionMap.end()) {
                            float randomValue = static_cast<float>(rand()) / RAND_MAX;
                            if (randomValue <= jsonEvent.chance) {
                                actionMap[jsonEvent.details.hitType.value()](event,
                                                                             jsonEvent.soundEffect);
                            }
                            return RE::BSEventNotifyControl::kContinue;
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessIdleEvent(const RE::TESEquipEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }

}
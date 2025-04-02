#include "WeaponEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundManager.h"
#include "Utility.h"

namespace SoundFX {

    std::unordered_map<std::string, WeaponEventHandler::EventAction> WeaponEventHandler::actionMap;

    void
        WeaponEventHandler::HandleAttackEvent(const EventVariant         &event,
                                              const std::string          &soundEffect,
                                              const RE::TESHitEvent::Flag flag) {
        if (const auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {
            if ((*hitEvent)->flags.any(flag)) { }
        }
    }

    void
        WeaponEventHandler::HandleNormalAttack(const EventVariant &event,
                                               const std::string  &soundEffect) {
        if (const auto hitEvent = std::get_if<const RE::TESHitEvent *>(&event)) {
            const RE::TESForm *item = RE::TESForm::LookupByID((*hitEvent)->source);
            if (!item) {
                return;
            }

            const auto *hit = item->As<RE::TESObjectWEAP>();
            if (!hit) {
                return;
            }

            if (hit->GetAttackDamage() > 0
                && !(*hitEvent)->flags.any(RE::TESHitEvent::Flag::kBashAttack,
                                           RE::TESHitEvent::Flag::kHitBlocked,
                                           RE::TESHitEvent::Flag::kPowerAttack,
                                           RE::TESHitEvent::Flag::kSneakAttack)) { }
        }
    }

    // InitAttackType temporary use for Filtering
    void
        WeaponEventHandler::InitializeAttackTypeHandlers() {
        actionMap["PowerAttack"] = [this](const EventVariant &event,
                                          const std::string  &soundEffect) {
            HandleAttackEvent(event, soundEffect, RE::TESHitEvent::Flag::kPowerAttack);
        };

        actionMap["SneakAttack"] = [this](const EventVariant &event,
                                          const std::string  &soundEffect) {
            HandleAttackEvent(event, soundEffect, RE::TESHitEvent::Flag::kSneakAttack);
        };

        actionMap["BashAttack"] = [this](const EventVariant &event,
                                         const std::string  &soundEffect) {
            HandleAttackEvent(event, soundEffect, RE::TESHitEvent::Flag::kBashAttack);
        };

        actionMap["NormalAttack"] = [this](const EventVariant &event,
                                           const std::string  &soundEffect) {
            HandleNormalAttack(event, soundEffect);
        };
    }

    void
        WeaponEventHandler::SetupWeaponTasks() {
        StartWeaponTask([this] { ProcessIdleTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESEquipEvent *event,
                                         RE::BSTEventSource<RE::TESEquipEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents({ProcessEquipEvent(event)});
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const RE::TESHitEvent *event,
                                         RE::BSTEventSource<RE::TESHitEvent> *) {
        return ProcessHitEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEvent(const SKSE::ActionEvent *event,
                                         RE::BSTEventSource<SKSE::ActionEvent> *) {
        return ProcessAttackEvent(event);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessEquipEvent(const RE::TESEquipEvent *event) const {

        if (event == nullptr || event->baseObject == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObject);
        if (!item) {
            spdlog::warn("Failed to lookup item for EquipEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->actor->GetObjectReference()
            == RE::PlayerCharacter::GetSingleton()->GetObjectReference()) {
            for (const auto &weapons = jsonLoader->getItems("weapons");
                 const auto &weaponEvents : weapons | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    weaponEvents.editorID, weaponEvents.pluginName);

                if (resolvedFormID == item->formID) {
                    for (const auto &jsonEvent : weaponEvents.events) {
                        if (jsonEvent.type == "Equip") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(weaponEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        {8029.8643f, -69089.37f, 4815.15f},
                                                        1000.0f,
                                                        jsonEvent.volume,
                                                        jsonEvent.isAbsoluteVolume,
                                                        true);
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
        WeaponEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const {

        if (event == nullptr || event->newContainer == 0) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const RE::TESForm *item = RE::TESForm::LookupByID(event->baseObj);
        if (!item) {
            spdlog::warn("Failed to lookup item for PickUpEvent.");
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &weapons = jsonLoader->getItems("weapons");
             const auto &weaponEvents : weapons | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(weaponEvents.editorID, weaponEvents.pluginName);

            if (resolvedFormID == item->formID) {
                for (const auto &jsonEvent : weaponEvents.events) {
                    if (jsonEvent.type == "PickUp") {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(weaponEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessHitEvent(const RE::TESHitEvent *event) const {

        if (event == nullptr || event->target == nullptr || event->source == 0
            || event->cause == nullptr) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->cause->GetObjectReference() == player->GetObjectReference()) {
            for (const auto &weapons = jsonLoader->getItems("weapons");
                 const auto &weaponEvents : weapons | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    weaponEvents.editorID, weaponEvents.pluginName);

                if (resolvedFormID == event->source) {
                    for (const auto &jsonEvent : weaponEvents.events) {
                        if (jsonEvent.type == "Hit") {
                            if (actionMap.contains(jsonEvent.details.hitType.value())) {
                                if (const float randomValue = GenerateRandomFloat();
                                    randomValue <= jsonEvent.chance) {
                                    actionMap[jsonEvent.details.hitType.value()](
                                        event, jsonEvent.soundEffect);
                                }
                                return RE::BSEventNotifyControl::kContinue;
                            }
                        }
                    }
                }
            }

            return RE::BSEventNotifyControl::kContinue;
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void
        WeaponEventHandler::ProcessIdleTask() const {

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        const auto *rightHandData = player->GetEquippedEntryData(false);
        const auto *leftHandData  = player->GetEquippedEntryData(true);

        if (!rightHandData && !leftHandData) {
            return;
        }

        auto checkAndPlaySound = [&](const RE::InventoryEntryData *handData) {
            if (!handData || !handData->GetObject()) {
                return;
            }

            const RE::FormID currentItem = handData->GetObject()->formID;
            if (currentItem == 0) {
                return;
            }

            for (const auto &weapons = jsonLoader->getItems("weapons");
                 const auto &weaponEvents : weapons | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    weaponEvents.editorID, weaponEvents.pluginName);

                if (resolvedFormID == currentItem) {
                    for (const auto &jsonEvent : weaponEvents.events) {
                        if (jsonEvent.type == "Idle" && player->AsActorState()->IsWeaponDrawn()) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(weaponEvents.name,
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
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);
    }

    RE::BSEventNotifyControl
        WeaponEventHandler::ProcessAttackEvent(const SKSE::ActionEvent *event) const {
        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (const auto *actorState = player->AsActorState();
            !actorState && !actorState->IsWeaponDrawn()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *rightHandData = player->GetEquippedEntryData(false);
        const auto *leftHandData  = player->GetEquippedEntryData(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::InventoryEntryData *handData) {
            if (!handData || !handData->GetObject()) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const RE::FormID currentItem = handData->GetObject()->formID;
            if (currentItem == 0) {
                return RE::BSEventNotifyControl::kContinue;
            }

            for (const auto &weapons = jsonLoader->getItems("weapons");
                 const auto &weaponEvents : weapons | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    weaponEvents.editorID, weaponEvents.pluginName);

                if (resolvedFormID == currentItem) {
                    for (const auto &jsonEvent : weaponEvents.events) {
                        if (jsonEvent.type == "Attack"
                            && event->type == SKSE::ActionEvent::Type::kWeaponSwing) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(weaponEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        {8029.8643f, -69089.37f, 4815.15f},
                                                        1000.0f,
                                                        jsonEvent.volume,
                                                        jsonEvent.isAbsoluteVolume,
                                                        true);
                            }
                            return RE::BSEventNotifyControl::kContinue;
                        }
                    }
                }
            }

            return RE::BSEventNotifyControl::kContinue;
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);

        return RE::BSEventNotifyControl::kContinue;
    }

}
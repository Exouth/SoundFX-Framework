#include "SpellEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundManager.h"
#include "Utility.h"

namespace SoundFX {

    void
        SpellEventHandler::SetupSpellTasks() {
        StartSpellTask([this] { ProcessIdleTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessEvent(const SKSE::ActionEvent *event,
                                        RE::BSTEventSource<SKSE::ActionEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents({ProcessDrawEvent(event),
                                                           ProcessSheatheEvent(event),
                                                           ProcessFireEvent(event),
                                                           ProcessCastEvent(event)});
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessDrawEvent(const SKSE::ActionEvent *event) const {

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

        const auto *rightHandData = player->GetEquippedObject(false);
        const auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            for (const auto &magic = jsonLoader->getItems("magicEffects");
                 const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Draw"
                            && event->type == SKSE::ActionEvent::Type::kEndDraw) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(magicEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        player->GetPosition(),
                                                        100.0f,
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

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessSheatheEvent(const SKSE::ActionEvent *event) const {

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

        const auto *rightHandData = player->GetEquippedObject(false);
        const auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            for (const auto &magic = jsonLoader->getItems("magicEffects");
                 const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Sheathe"
                            && event->type == SKSE::ActionEvent::Type::kEndSheathe) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(magicEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        player->GetPosition(),
                                                        100.0f,
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

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessFireEvent(const SKSE::ActionEvent *event) const {

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

        const auto *rightHandData = player->GetEquippedObject(false);
        const auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            for (const auto &magic = jsonLoader->getItems("magicEffects");
                 const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Fire"
                            && event->type == SKSE::ActionEvent::Type::kSpellFire) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(magicEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        player->GetPosition(),
                                                        100.0f,
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

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessCastEvent(const SKSE::ActionEvent *event) const {

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

        const auto *rightHandData = player->GetEquippedObject(false);
        const auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            for (const auto &magic = jsonLoader->getItems("magicEffects");
                 const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Cast"
                            && event->type == SKSE::ActionEvent::Type::kSpellCast) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                spdlog::info("volume: {}", jsonEvent.volume);
                                SoundManager::PlaySound(magicEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        player->GetPosition(),
                                                        100.0f,
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

    void
        SpellEventHandler::ProcessIdleTask() const {

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        const auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return;
        }

        const auto *rightHandData = player->GetEquippedObject(false);
        const auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return;
            }

            for (const auto &magic = jsonLoader->getItems("magicEffects");
                 const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Idle" && actorState->IsWeaponDrawn()) {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(magicEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        player->GetPosition(),
                                                        100.0f,
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
}
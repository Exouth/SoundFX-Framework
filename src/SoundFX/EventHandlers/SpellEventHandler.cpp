#include "SpellEventHandler.h"

namespace SoundFX {

    void
        SpellEventHandler::SetupSpellTasks() {
        StartSpellTask([this]() { ProcessIdleTask(); }, true);
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
        SpellEventHandler::ProcessDrawEvent(const SKSE::ActionEvent *event) {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *rightHandData = player->GetEquippedObject(false);
        auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &[magicName, magicEvents] : magic) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Draw"
                            && event->type == SKSE::ActionEvent::Type::kEndDraw) {
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
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessSheatheEvent(const SKSE::ActionEvent *event) {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *rightHandData = player->GetEquippedObject(false);
        auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &[magicName, magicEvents] : magic) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Sheathe"
                            && event->type == SKSE::ActionEvent::Type::kEndSheathe) {
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
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessFireEvent(const SKSE::ActionEvent *event) {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *rightHandData = player->GetEquippedObject(false);
        auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &[magicName, magicEvents] : magic) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Fire"
                            && event->type == SKSE::ActionEvent::Type::kSpellFire) {
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
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessCastEvent(const SKSE::ActionEvent *event) {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *rightHandData = player->GetEquippedObject(false);
        auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return RE::BSEventNotifyControl::kContinue;
            }

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &[magicName, magicEvents] : magic) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Cast"
                            && event->type == SKSE::ActionEvent::Type::kSpellCast) {
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
        };

        checkAndPlaySound(rightHandData);
        checkAndPlaySound(leftHandData);

        return RE::BSEventNotifyControl::kContinue;
    }

    void
        SpellEventHandler::ProcessIdleTask() {

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return;
        }

        auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
            return;
        }

        auto *rightHandData = player->GetEquippedObject(false);
        auto *leftHandData  = player->GetEquippedObject(true);

        if (!rightHandData && !leftHandData) {
            return;
        }

        auto checkAndPlaySound = [&](const RE::TESForm *handData) {
            if (!handData || !handData->formID) {
                return;
            }

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &[magicName, magicEvents] : magic) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Idle" && actorState->IsWeaponDrawn()) {
                            float randomValue = static_cast<float>(rand()) / RAND_MAX;
                            if (randomValue <= jsonEvent.chance) {
                                PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
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

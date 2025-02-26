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
        SpellEventHandler::ProcessDrawEvent(const SKSE::ActionEvent *event) const {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
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

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Draw"
                            && event->type == SKSE::ActionEvent::Type::kEndDraw) {
                            const float randomValue = GenerateRandomFloat();
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
        SpellEventHandler::ProcessSheatheEvent(const SKSE::ActionEvent *event) const {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
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

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Sheathe"
                            && event->type == SKSE::ActionEvent::Type::kEndSheathe) {
                            const float randomValue = GenerateRandomFloat();
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
        SpellEventHandler::ProcessFireEvent(const SKSE::ActionEvent *event) const {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
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

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Fire"
                            && event->type == SKSE::ActionEvent::Type::kSpellFire) {
                            const float randomValue = GenerateRandomFloat();
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
        SpellEventHandler::ProcessCastEvent(const SKSE::ActionEvent *event) const {

        if (!event || !event->actor || event->actor != RE::PlayerCharacter::GetSingleton()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *actorState = player->AsActorState();
        if (!actorState && !actorState->IsWeaponDrawn()) {
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

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Cast"
                            && event->type == SKSE::ActionEvent::Type::kSpellCast) {
                            const float randomValue = GenerateRandomFloat();
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

            const auto &magic = jsonLoader.getItems("magicEffects");
            for (const auto &magicEvents : magic | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    magicEvents.editorID, magicEvents.pluginName);

                if (resolvedFormID == handData->formID) {
                    for (const auto &jsonEvent : magicEvents.events) {
                        if (jsonEvent.type == "Idle" && actorState->IsWeaponDrawn()) {
                            const float randomValue = GenerateRandomFloat();
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

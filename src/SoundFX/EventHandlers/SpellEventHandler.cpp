#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"
#include "SpellEventHandler.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessEvent(const SKSE::ActionEvent *event,
                                        RE::BSTEventSource<SKSE::ActionEvent> *) {
        return ProcessDrawEvent(event);
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessDrawEvent(const SKSE::ActionEvent *event) {

        if (!event || !event->actor) {
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
                        if (jsonEvent.type == "Draw" && event->type == SKSE::ActionEvent::Type::kEndDraw) {
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
}

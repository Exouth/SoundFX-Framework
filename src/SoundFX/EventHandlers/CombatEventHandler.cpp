#include "CombatEventHandler.h"
#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessEvent(const RE::TESCombatEvent *event,
                                         RE::BSTEventSource<RE::TESCombatEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents({ProcessStartCombatEvent(event)});
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessStartCombatEvent(const RE::TESCombatEvent *event) {

        if (event == nullptr || event->newState.any(RE::ACTOR_COMBAT_STATE::kNone)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!event->targetActor || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &combats = jsonLoader.getItems("combats");
        for (const auto &[combatName, combatEvents] : combats) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Start"
                        && event->newState.any(RE::ACTOR_COMBAT_STATE::kCombat)) {
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
        CombatEventHandler::ProcessSearchCombatEvent(const RE::TESCombatEvent *event) {

        if (event == nullptr || event->newState.any(RE::ACTOR_COMBAT_STATE::kNone)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!event->targetActor || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &combats = jsonLoader.getItems("combats");
        for (const auto &[combatName, combatEvents] : combats) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Start"
                        && event->newState.any(RE::ACTOR_COMBAT_STATE::kSearching)) {
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
}
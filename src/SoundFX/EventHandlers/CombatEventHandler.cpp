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

        return RE::BSEventNotifyControl::kContinue;
    }
}
#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class CombatEventHandler : public RE::BSTEventSink<RE::TESCombatEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit CombatEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESCombatEvent *event, RE::BSTEventSource<RE::TESCombatEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessStartCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessSearchCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessStopCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessDieCombatEvent(const RE::TESCombatEvent *event);

        bool
            IsPlayerInvolvedInCombat(RE::Actor *eventActor, RE::PlayerCharacter *player);
    };
}
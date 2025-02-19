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
    };
}
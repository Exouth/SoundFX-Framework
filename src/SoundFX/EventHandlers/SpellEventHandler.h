#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class SpellEventHandler : public RE::BSTEventSink<RE::TESEquipEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit SpellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event, RE::BSTEventSource<RE::TESEquipEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event);
    };

}
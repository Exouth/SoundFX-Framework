#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class WeaponEventHandler :
        public RE::BSTEventSink<RE::TESActivateEvent>,
        public RE::BSTEventSink<RE::TESEquipEvent>,
        public RE::BSTEventSink<RE::TESHitEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit WeaponEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESActivateEvent *event,
                         RE::BSTEventSource<RE::TESActivateEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event, RE::BSTEventSource<RE::TESEquipEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessActivateEvent(const RE::TESActivateEvent *event);
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event);
        RE::BSEventNotifyControl
            ProcessHitEvent(const RE::TESHitEvent *event);
    };

}
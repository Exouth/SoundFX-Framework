#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class WeaponEventHandler :
        public RE::BSTEventSink<RE::TESContainerChangedEvent>,
        public RE::BSTEventSink<RE::TESEquipEvent>,
        public RE::BSTEventSink<RE::TESHitEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit WeaponEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event, RE::BSTEventSource<RE::TESEquipEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event);
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event);
        RE::BSEventNotifyControl
            ProcessHitEvent(const RE::TESHitEvent *event);
    };

}
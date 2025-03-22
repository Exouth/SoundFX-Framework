#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class ArmorEventHandler final :
        public RE::BSTEventSink<RE::TESContainerChangedEvent>,
        public RE::BSTEventSink<RE::TESEquipEvent> {
        JSONLoader *jsonLoader;

      public:
        explicit ArmorEventHandler(JSONLoader *loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) override;
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event,
                         RE::BSTEventSource<RE::TESEquipEvent> *) override;

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const;
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event) const;
        RE::BSEventNotifyControl
            ProcessUnequipEvent(const RE::TESEquipEvent *event) const;
    };
}
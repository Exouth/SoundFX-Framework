#pragma once

namespace SoundFX {

    class ArmorEventHandler :
        public RE::BSTEventSink<RE::TESContainerChangedEvent>,
        public RE::BSTEventSink<RE::TESEquipEvent> {
        JSONLoader   &jsonLoader;

      public:
        explicit ArmorEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event,
                         RE::BSTEventSource<RE::TESEquipEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event);
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event);
        RE::BSEventNotifyControl
            ProcessUnequipEvent(const RE::TESEquipEvent *event);
    };
}
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

        void
            InitializeAttackTypeHandlers();

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event, RE::BSTEventSource<RE::TESEquipEvent> *);
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> *);

      private:
        using EventVariant = std::variant<const RE::TESHitEvent *, const RE::TESEquipEvent *>;

        using EventAction = std::function<void(EventVariant, const std::string &)>;
        static std::unordered_map<std::string, EventAction> actionMap;

        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event);
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event);
        RE::BSEventNotifyControl
            ProcessHitEvent(const RE::TESHitEvent *event);
        RE::BSEventNotifyControl
            ProcessIdleEvent(const RE::TESEquipEvent *event);
    };

}
#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class WeaponEventHandler final :
        public RE::BSTEventSink<RE::TESContainerChangedEvent>,
        public RE::BSTEventSink<RE::TESEquipEvent>,
        public RE::BSTEventSink<RE::TESHitEvent>,
        public RE::BSTEventSink<SKSE::ActionEvent> {
        JSONLoader   *jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit WeaponEventHandler(JSONLoader *loader) : jsonLoader(loader) {
        }

        void
            SetupWeaponTasks();

        void
            InitializeAttackTypeHandlers();

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) override;
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESEquipEvent *event,
                         RE::BSTEventSource<RE::TESEquipEvent> *) override;
        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESHitEvent *event,
                         RE::BSTEventSource<RE::TESHitEvent> *) override;
        RE::BSEventNotifyControl
            ProcessEvent(const SKSE::ActionEvent *event,
                         RE::BSTEventSource<SKSE::ActionEvent> *) override;

      private:
        using EventVariant = std::variant<const RE::TESHitEvent *, const RE::TESEquipEvent *>;
        using EventAction  = std::function<void(EventVariant, const std::string &)>;

        static std::unordered_map<std::string, EventAction> actionMap;

        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const;
        RE::BSEventNotifyControl
            ProcessEquipEvent(const RE::TESEquipEvent *event) const;
        RE::BSEventNotifyControl
            ProcessHitEvent(const RE::TESHitEvent *event) const;
        RE::BSEventNotifyControl
            ProcessAttackEvent(const SKSE::ActionEvent *event) const;

        void
            ProcessIdleTask() const;

        void
            StartWeaponTask(const std::function<void()> &task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }

        void
            HandleAttackEvent(const EventVariant   &event,
                              const std::string    &soundEffect,
                              RE::TESHitEvent::Flag flag);
        void
            HandleNormalAttack(const EventVariant &event, const std::string &soundEffect);
    };

}
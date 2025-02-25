#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class CombatEventHandler : public RE::BSTEventSink<RE::TESCombatEvent> {
        JSONLoader   &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit CombatEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESCombatEvent *event, RE::BSTEventSource<RE::TESCombatEvent> *);

        void
            SetupCombatTasks();

      private:
        static constexpr float MAX_DISTANCE = 500.0f;

        RE::BSEventNotifyControl
            ProcessStartCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessSearchCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessStopCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessDieCombatEvent(const RE::TESCombatEvent *event);
        RE::BSEventNotifyControl
            ProcessFleeCombatEvent(const RE::TESCombatEvent *event);

        void
            ProcessKillMoveCombatTask();

        void
            StartCombatTask(std::function<void()> task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }

        bool
            IsPlayerInvolvedInCombat(RE::Actor *eventActor, RE::PlayerCharacter *player);
        bool
            IsActorFleeing(const RE::Actor *eventActor) const;
        std::vector<RE::Actor *>
            GetCombatTargets(RE::Actor *eventActor);
        RE::Actor *
            FindActorByFormID(RE::Actor                      *playerActor,
                              RE::FormID                      formID,
                              const std::vector<RE::Actor *> &actors);
        RE::Actor *
            GetCombatTargetByFormID(RE::Actor *eventActor, RE::FormID formID);
        float
            GetDistanceBetweenActors(RE::Actor *actor1, RE::Actor *actor2);
    };
}
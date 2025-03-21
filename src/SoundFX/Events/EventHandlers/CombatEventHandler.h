#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class CombatEventHandler final : public RE::BSTEventSink<RE::TESCombatEvent> {
        JSONLoader   *jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit CombatEventHandler(JSONLoader *loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESCombatEvent *event,
                         RE::BSTEventSource<RE::TESCombatEvent> *) override;

        void
            SetupCombatTasks();

      private:
        static constexpr float MAX_DISTANCE = 500.0f;

        RE::BSEventNotifyControl
            ProcessStartCombatEvent(const RE::TESCombatEvent *event) const;
        RE::BSEventNotifyControl
            ProcessSearchCombatEvent(const RE::TESCombatEvent *event) const;
        RE::BSEventNotifyControl
            ProcessStopCombatEvent(const RE::TESCombatEvent *event) const;
        RE::BSEventNotifyControl
            ProcessDieCombatEvent(const RE::TESCombatEvent *event) const;
        RE::BSEventNotifyControl
            ProcessFleeCombatEvent(const RE::TESCombatEvent *event) const;

        void
            ProcessKillMoveCombatTask() const;

        void
            StartCombatTask(const std::function<void()> &task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }

        static bool
            IsPlayerInvolvedInCombat(const RE::Actor           *eventActor,
                                     const RE::PlayerCharacter *player);
        static bool
            IsActorFleeing(const RE::Actor *eventActor);
        static std::vector<RE::Actor *>
            GetCombatTargets(const RE::Actor *eventActor);
        static RE::Actor *
            FindActorByFormID(const RE::Actor                *playerActor,
                              RE::FormID                      formID,
                              const std::vector<RE::Actor *> &actors);
        static RE::Actor *
            GetCombatTargetByFormID(const RE::Actor *eventActor, RE::FormID formID);
        static float
            GetDistanceBetweenActors(const RE::Actor *actor1, const RE::Actor *actor2);
    };
}
#pragma once

#include "TaskScheduler.h"

namespace SoundFX {

    class NpcInteractionEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
        JSONLoader   &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit NpcInteractionEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        void
            SetupNpcInteractionTasks();

        void
            InitializeOnlyAtTypeHandlers();

        RE::BSEventNotifyControl
            ProcessEvent(const RE::MenuOpenCloseEvent *event,
                         RE::BSTEventSource<RE::MenuOpenCloseEvent> *) override;

      private:
        RE::TESTopic *lastParentTopic = nullptr;

        using EventAction =
            std::function<void(const std::string &, const std::string &, const std::string &)>;

        static std::unordered_map<std::string, EventAction> actionMap;

        RE::BSEventNotifyControl
            ProcessDialogOpenEvent(const RE::MenuOpenCloseEvent *event);

        void
            ProcessDialogTopicTask();

        void
            StartNpcInteractionTask(std::function<void()> task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }

        RE::NiPoint3
            GetActorForwardVector(RE::Actor *actor);

        std::unordered_map<RE::FormID, RE::Actor *>
            GetNpcsInPlayerFOV(RE::Actor *player, float range, float fovAngle);
    };
}
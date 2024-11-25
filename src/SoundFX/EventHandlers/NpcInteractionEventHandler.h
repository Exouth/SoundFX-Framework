#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class NpcInteractionEventHandler {
        JSONLoader   &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit NpcInteractionEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        void
            SetupNpcInteractionTasks();

      private:
        void
            ProcessDialogOpenTask();

        void
            StartNpcInteractionTask(std::function<void()> task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }
    };
}
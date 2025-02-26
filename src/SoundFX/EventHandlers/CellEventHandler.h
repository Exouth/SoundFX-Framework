#pragma once

#include "TaskScheduler.h"

namespace SoundFX {

    class CellEventHandler {
        JSONLoader &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit CellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        void
            SetupCellTasks();

      private:
        void
            ProcessCellEnterTask();
        void
            ProcessAmbienceSoundTask();

        void
            StartCellTask(std::function<void()> task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }
    };
}
#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class CellEventHandler {
        JSONLoader   &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit CellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        void
            SetupCellTasks();

      private:
        void
            ProcessCellEnterTask() const;
        void
            ProcessAmbienceSoundTask() const;

        void
            StartCellTask(const std::function<void()> &task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }
    };
}
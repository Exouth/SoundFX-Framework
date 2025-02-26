#pragma once

#include "TaskScheduler.h"

namespace SoundFX {

    class SpellEventHandler : public RE::BSTEventSink<SKSE::ActionEvent> {
        JSONLoader   &jsonLoader;
        TaskScheduler scheduler;

      public:
        explicit SpellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        void
            SetupSpellTasks();

        RE::BSEventNotifyControl
            ProcessEvent(const SKSE::ActionEvent *event,
                         RE::BSTEventSource<SKSE::ActionEvent> *) override;

      private:
        RE::BSEventNotifyControl
            ProcessDrawEvent(const SKSE::ActionEvent *event) const;
        RE::BSEventNotifyControl
            ProcessSheatheEvent(const SKSE::ActionEvent *event) const;
        RE::BSEventNotifyControl
            ProcessFireEvent(const SKSE::ActionEvent *event) const;
        RE::BSEventNotifyControl
            ProcessCastEvent(const SKSE::ActionEvent *event) const;

        void
            ProcessIdleTask() const;

        void
            StartSpellTask(const std::function<void()> &task, bool repeat = false) {
            scheduler.AddTask(task, repeat);
        }
    };

}
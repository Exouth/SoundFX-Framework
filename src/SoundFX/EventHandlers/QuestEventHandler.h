#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class QuestEventHandler : public RE::BSTEventSink<RE::TESQuestStartStopEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit QuestEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESQuestStartStopEvent *event,
                         RE::BSTEventSource<RE::TESQuestStartStopEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessStartQuestEvent(const RE::TESQuestStartStopEvent *event);
        RE::BSEventNotifyControl
            ProcessEndQuestEvent(const RE::TESQuestStartStopEvent *event);
    };
}
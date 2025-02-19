#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class QuestEventHandler : public RE::BSTEventSink<RE::TESQuestStageEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit QuestEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESQuestStageEvent *event,
                         RE::BSTEventSource<RE::TESQuestStageEvent> *);

      private:
        std::uint16_t
            GetFirstActiveStage(RE::TESQuest *quest);

        RE::BSEventNotifyControl
            ProcessStartQuestEvent(const RE::TESQuestStageEvent *event);
        RE::BSEventNotifyControl
            ProcessEndQuestEvent(const RE::TESQuestStageEvent *event);
    };
}
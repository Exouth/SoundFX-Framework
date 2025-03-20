#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class QuestEventHandler final : public RE::BSTEventSink<RE::TESQuestStageEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit QuestEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESQuestStageEvent *event,
                         RE::BSTEventSource<RE::TESQuestStageEvent> *) override;

      private:
        static std::uint16_t
            GetFirstActiveStage(const RE::TESQuest *quest);

        RE::BSEventNotifyControl
            ProcessStartQuestEvent(const RE::TESQuestStageEvent *event) const;
        RE::BSEventNotifyControl
            ProcessEndQuestEvent(const RE::TESQuestStageEvent *event) const;
    };
}
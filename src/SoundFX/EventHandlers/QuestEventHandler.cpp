#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "QuestEventHandler.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessEvent(const RE::TESQuestStageEvent *event,
                                        RE::BSTEventSource<RE::TESQuestStageEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessStartQuestEvent(event), ProcessEndQuestEvent(event)});
    }

    std::uint16_t
        QuestEventHandler::GetFirstActiveStage(RE::TESQuest *quest) {
        if (!quest || !quest->executedStages) {
            return 0;  // Fallback
        }

        auto          stages        = quest->executedStages;
        std::uint16_t fallbackStage = 0;  // Fallback

        for (const auto &stage : *stages) {
            if (!stage) {
                continue;
            }

            auto stageIndex = stage.data.index;
            if (stageIndex == 0) {
                fallbackStage = 10;
            } else if (fallbackStage == 0 || stageIndex < fallbackStage) {
                fallbackStage = stageIndex;
            }
        }

        return fallbackStage;
    }

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessStartQuestEvent(const RE::TESQuestStageEvent *event) {

        if (!event || !event->formID) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *quest = RE::TESForm::LookupByID<RE::TESQuest>(event->formID);
        if (!quest) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &quests = jsonLoader.getItems("quests");
        for (const auto &[questName, questEvents] : quests) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(questEvents.editorID, questEvents.pluginName);

            if (resolvedFormID == event->formID) {
                for (const auto &jsonEvent : questEvents.events) {
                    auto startStage = GetFirstActiveStage(quest);
                    if (jsonEvent.type == "Start" && event->stage == startStage) {
                        float randomValue = static_cast<float>(rand()) / RAND_MAX;
                        if (randomValue <= jsonEvent.chance) {
                            PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessEndQuestEvent(const RE::TESQuestStageEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}
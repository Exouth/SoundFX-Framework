#include "QuestEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessEvent(const RE::TESQuestStageEvent *event,
                                        RE::BSTEventSource<RE::TESQuestStageEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessStartQuestEvent(event), ProcessEndQuestEvent(event)});
    }

    // !!!Maybe fix later if it makes Problems with Stages
    std::uint16_t
        QuestEventHandler::GetFirstActiveStage(const RE::TESQuest *quest) {
        if (!quest || !quest->executedStages) {
            return 0;  // Fallback
        }

        const auto    stages        = quest->executedStages;
        std::uint16_t fallbackStage = 0;  // Fallback

        for (const auto &stage : *stages) {
            if (!stage) {
                continue;
            }

            const auto stageIndex = stage.data.index;
            if (stageIndex == 0) {
                fallbackStage = 10;
            } else if (fallbackStage == 0 || stageIndex < fallbackStage) {
                fallbackStage = stageIndex;
            }
        }

        return fallbackStage;
    }

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessStartQuestEvent(const RE::TESQuestStageEvent *event) const {

        if (!event || !event->formID) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *quest = RE::TESForm::LookupByID<RE::TESQuest>(event->formID);
        if (!quest) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &quests = jsonLoader.getItems("quests");
        for (const auto &questEvents : quests | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(questEvents.editorID, questEvents.pluginName);

            if (resolvedFormID == event->formID) {
                for (const auto &jsonEvent : questEvents.events) {
                    const auto startStage = GetFirstActiveStage(quest);
                    if (jsonEvent.type == "Start" && event->stage == startStage) {
                        const float randomValue = GenerateRandomFloat();
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
        QuestEventHandler::ProcessEndQuestEvent(const RE::TESQuestStageEvent *event) const {

        if (!event || !event->formID) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *quest = RE::TESForm::LookupByID<RE::TESQuest>(event->formID);
        if (!quest) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto &quests = jsonLoader.getItems("quests");
        for (const auto &questEvents : quests | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(questEvents.editorID, questEvents.pluginName);

            if (resolvedFormID == event->formID) {
                for (const auto &jsonEvent : questEvents.events) {
                    if (jsonEvent.type == "End" && quest->IsCompleted()) {
                        const float randomValue = GenerateRandomFloat();
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
}
#include "QuestEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundManager.h"
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

            if (const auto stageIndex = stage.data.index; stageIndex == 0) {
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

        for (const auto &quests = jsonLoader->getItems("quests");
             const auto &questEvents : quests | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(questEvents.editorID, questEvents.pluginName);

            if (resolvedFormID == event->formID) {
                for (const auto &jsonEvent : questEvents.events) {
                    const auto startStage = GetFirstActiveStage(quest);
                    if (jsonEvent.type == "Start" && event->stage == startStage) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(questEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
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

        for (const auto &quests = jsonLoader->getItems("quests");
             const auto &questEvents : quests | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(questEvents.editorID, questEvents.pluginName);

            if (resolvedFormID == event->formID) {
                for (const auto &jsonEvent : questEvents.events) {
                    if (jsonEvent.type == "End" && quest->IsCompleted()) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(questEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }
}
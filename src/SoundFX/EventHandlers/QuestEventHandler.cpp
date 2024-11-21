#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "QuestEventHandler.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessEvent(const RE::TESQuestStartStopEvent *event,
                                           RE::BSTEventSource<RE::TESQuestStartStopEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents(
            {ProcessStartQuestEvent(event), ProcessEndQuestEvent(event)});
    }

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessStartQuestEvent(const RE::TESQuestStartStopEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        QuestEventHandler::ProcessEndQuestEvent(const RE::TESQuestStartStopEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}
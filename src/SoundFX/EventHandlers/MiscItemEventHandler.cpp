#include "MiscItemEventHandler.h"
#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                        RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        MiscItemEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}
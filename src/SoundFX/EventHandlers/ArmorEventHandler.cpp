#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "ArmorEventHandler.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessEvent(const RE::TESContainerChangedEvent *event,
                                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) {
        return ProcessPickUpEvent(event);
    }

    RE::BSEventNotifyControl
        ArmorEventHandler::ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}

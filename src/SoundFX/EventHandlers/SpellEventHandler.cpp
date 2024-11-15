#include "EventHandlerManager.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"
#include "SpellEventHandler.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessEvent(const RE::TESEquipEvent *event,
                                        RE::BSTEventSource<RE::TESEquipEvent> *) {
        return ProcessEquipEvent(event);
    }

    RE::BSEventNotifyControl
        SpellEventHandler::ProcessEquipEvent(const RE::TESEquipEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}

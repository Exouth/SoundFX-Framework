#include "CellEventHandler.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

    RE::BSEventNotifyControl
        CellEventHandler::ProcessEvent(const RE::TESCellAttachDetachEvent *event,
                                       RE::BSTEventSource<RE::TESCellAttachDetachEvent> *) {
        return ProcessCellEnterEvent(event);
    }

    RE::BSEventNotifyControl
        CellEventHandler::ProcessCellEnterEvent(const RE::TESCellAttachDetachEvent *event) {

        return RE::BSEventNotifyControl::kContinue;
    }
}
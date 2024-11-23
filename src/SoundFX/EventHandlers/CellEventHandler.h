#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class CellEventHandler : public RE::BSTEventSink<RE::TESCellAttachDetachEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit CellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESCellAttachDetachEvent *event,
                         RE::BSTEventSource<RE::TESCellAttachDetachEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessCellEnterEvent(const RE::TESCellAttachDetachEvent *event);
    };
}
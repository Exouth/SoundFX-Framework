#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class ArmorEventHandler : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
        JSONLoader   &jsonLoader;

      public:
        explicit ArmorEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event);
    };
}
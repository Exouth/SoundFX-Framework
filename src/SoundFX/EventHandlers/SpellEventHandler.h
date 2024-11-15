#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class SpellEventHandler : public RE::BSTEventSink<SKSE::ActionEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit SpellEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const SKSE::ActionEvent *event, RE::BSTEventSource<SKSE::ActionEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessDrawEvent(const SKSE::ActionEvent *event);
    };

}
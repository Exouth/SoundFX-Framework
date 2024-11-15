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
        RE::BSEventNotifyControl
            ProcessSheatheEvent(const SKSE::ActionEvent *event);
        RE::BSEventNotifyControl
            ProcessFireEvent(const SKSE::ActionEvent *event);
        RE::BSEventNotifyControl
            ProcessCastEvent(const SKSE::ActionEvent *event);
    };

}
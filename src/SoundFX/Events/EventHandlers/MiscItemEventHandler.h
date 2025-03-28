#pragma once

#include "JSONLoader.h"

namespace SoundFX {

    class MiscItemEventHandler final : public RE::BSTEventSink<RE::TESContainerChangedEvent> {
        JSONLoader *jsonLoader;

      public:
        explicit MiscItemEventHandler(JSONLoader *loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *) override;

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event) const;
        RE::BSEventNotifyControl
            ProcessDropEvent(const RE::TESContainerChangedEvent *event) const;
    };
}
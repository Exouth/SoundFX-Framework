#pragma once

namespace SoundFX {

    class MiscItemEventHandler :
        public RE::BSTEventSink<RE::TESContainerChangedEvent> {
        JSONLoader &jsonLoader;

      public:
        explicit MiscItemEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::TESContainerChangedEvent *event,
                         RE::BSTEventSource<RE::TESContainerChangedEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessPickUpEvent(const RE::TESContainerChangedEvent *event);
        RE::BSEventNotifyControl
            ProcessDropEvent(const RE::TESContainerChangedEvent *event);
    };
}
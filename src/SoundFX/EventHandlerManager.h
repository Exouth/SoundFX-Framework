#pragma once

#include "EventHandlers/WeaponEventHandler.h"
#include "JSONLoader.h"

namespace SoundFX {

    class EventHandlerManager {
      public:
        explicit EventHandlerManager(JSONLoader &jsonLoader);

        void
            InitializeEventHandlers();

        static RE::BSEventNotifyControl
            ProcessMultipleEvents(std::initializer_list<RE::BSEventNotifyControl> events);

      private:
        JSONLoader                         &jsonLoader;
        std::unique_ptr<WeaponEventHandler> weaponEventHandler;

        template <typename HandlerType, typename... EventTypes>
        void
            RegisterMultipleEventHandlers(RE::ScriptEventSourceHolder *eventSource,
                                          HandlerType                 *handler,
                                          EventTypes... events);

        template <typename EventType, typename HandlerType>
        void
            RegisterEventHandler(RE::ScriptEventSourceHolder *eventSource, HandlerType *handler);
    };

    template <typename HandlerType, typename... EventTypes>
    void
        EventHandlerManager::RegisterMultipleEventHandlers(RE::ScriptEventSourceHolder *eventSource,
                                                           HandlerType                 *handler,
                                                           EventTypes... events) {
        (RegisterEventHandler<EventTypes>(eventSource, handler), ...);
    }

    template <typename EventType, typename HandlerType>
    void
        EventHandlerManager::RegisterEventHandler(RE::ScriptEventSourceHolder *eventSource,
                                                  HandlerType                 *handler) {
        eventSource->AddEventSink<EventType>(handler);
        spdlog::info("Registered handler for event type: {}", typeid(EventType).name());
    }

}
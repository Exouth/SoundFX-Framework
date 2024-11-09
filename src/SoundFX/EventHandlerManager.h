#pragma once

#include "EventHandlers/WeaponEventHandler.h"
#include "JSONLoader.h"
#include <memory>

namespace SoundFX {

    class EventHandlerManager {
      public:
        explicit EventHandlerManager(JSONLoader &jsonLoader);

        void
            InitializeEventHandlers();

      private:
        JSONLoader &jsonLoader;

        std::unique_ptr<WeaponEventHandler>   weaponEventHandler;

        template <typename EventType, typename HandlerType>
        void
            RegisterEventHandler(RE::ScriptEventSourceHolder *eventSource, HandlerType *handler);
    };
}
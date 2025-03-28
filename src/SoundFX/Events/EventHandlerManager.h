#pragma once

#include "EventHandlers/ArmorEventHandler.h"
#include "EventHandlers/CellEventHandler.h"
#include "EventHandlers/CombatEventHandler.h"
#include "EventHandlers/MiscItemEventHandler.h"
#include "EventHandlers/NpcInteractionEventHandler.h"
#include "EventHandlers/QuestEventHandler.h"
#include "EventHandlers/SpellEventHandler.h"
#include "EventHandlers/WeaponEventHandler.h"

namespace SoundFX {

    class EventHandlerManager {
      public:
        explicit EventHandlerManager(JSONLoader *jsonLoader);

        void
            InitializeEventHandlers();

        static RE::BSEventNotifyControl
            ProcessMultipleEvents(std::initializer_list<RE::BSEventNotifyControl> events);

      private:
        JSONLoader                                 *jsonLoader;
        std::unique_ptr<WeaponEventHandler>         weaponEventHandler;
        std::unique_ptr<SpellEventHandler>          spellEventHandler;
        std::unique_ptr<ArmorEventHandler>          armorEventHandler;
        std::unique_ptr<MiscItemEventHandler>       miscItemEventHandler;
        std::unique_ptr<QuestEventHandler>          questEventHandler;
        std::unique_ptr<CellEventHandler>           cellEventHandler;
        std::unique_ptr<NpcInteractionEventHandler> npcInteractionEventHandler;
        std::unique_ptr<CombatEventHandler>         combatEventHandler;

        template <typename EventSourceType, typename HandlerType, typename... EventTypes>
        void
            RegisterMultipleEventHandlers(EventSourceType *eventSource,
                                          HandlerType     *handler,
                                          const EventTypes &...events);

        template <typename EventSourceType, typename EventType, typename HandlerType>
        static void
            RegisterEventHandler(EventSourceType *eventSource, HandlerType *handler);
    };

    template <typename EventSourceType, typename HandlerType, typename... EventTypes>
    void
        EventHandlerManager::RegisterMultipleEventHandlers(EventSourceType *eventSource,
                                                           HandlerType     *handler,
                                                           const EventTypes &...events) {
        (RegisterEventHandler<EventSourceType, EventTypes>(eventSource, handler), ...);
    }

    template <typename EventSourceType, typename EventType, typename HandlerType>
    void
        EventHandlerManager::RegisterEventHandler(EventSourceType *eventSource,
                                                  HandlerType     *handler) {
        eventSource->template AddEventSink<EventType>(handler);
        spdlog::info("Registered handler for event type: {}", typeid(EventType).name());
    }

}
#include "EventHandlerManager.h"

namespace SoundFX {

    EventHandlerManager::EventHandlerManager(JSONLoader &jsonLoader) : jsonLoader(jsonLoader) {
    }

    void
        EventHandlerManager::InitializeEventHandlers() {
        auto eventSource = RE::ScriptEventSourceHolder::GetSingleton();

        weaponEventHandler = std::make_unique<WeaponEventHandler>(jsonLoader);
        RegisterEventHandler<RE::TESEquipEvent>(eventSource, weaponEventHandler.get());
        RegisterEventHandler<RE::TESContainerChangedEvent>(eventSource, weaponEventHandler.get());
    }

    template <typename EventType, typename HandlerType>
    void
        EventHandlerManager::RegisterEventHandler(RE::ScriptEventSourceHolder *eventSource,
                                                  HandlerType                 *handler) {
        eventSource->AddEventSink<EventType>(handler);
        spdlog::info("Registered handler for event type: {}", typeid(EventType).name());
    }

}
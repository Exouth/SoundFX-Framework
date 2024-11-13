#include "EventHandlerManager.h"

namespace SoundFX {

    EventHandlerManager::EventHandlerManager(JSONLoader &jsonLoader) : jsonLoader(jsonLoader) {
    }

    void
        EventHandlerManager::InitializeEventHandlers() {
        auto eventSource     = RE::ScriptEventSourceHolder::GetSingleton();
        auto eventSourceSKSE = SKSE::GetActionEventSource();

        weaponEventHandler = std::make_unique<WeaponEventHandler>(jsonLoader);
        weaponEventHandler->InitializeAttackTypeHandlers();
        weaponEventHandler->SetupWeaponTasks();

        RegisterMultipleEventHandlers(eventSource,
                                      weaponEventHandler.get(),
                                      RE::TESEquipEvent {},
                                      RE::TESContainerChangedEvent {},
                                      RE::TESHitEvent {});

        RegisterMultipleEventHandlers(
            eventSourceSKSE, weaponEventHandler.get(), SKSE::ActionEvent {});
    }

    RE::BSEventNotifyControl
        EventHandlerManager::ProcessMultipleEvents(
            std::initializer_list<RE::BSEventNotifyControl> events) {
        for (auto eventResult : events) {
            if (eventResult == RE::BSEventNotifyControl::kStop) {
                return RE::BSEventNotifyControl::kStop;
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

}
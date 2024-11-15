#include "EventHandlerManager.h"
#include "EventHandlers/SpellEventHandler.h"

namespace SoundFX {

    EventHandlerManager::EventHandlerManager(JSONLoader &jsonLoader) : jsonLoader(jsonLoader) {
    }

    void
        EventHandlerManager::InitializeEventHandlers() {
        auto eventSource     = RE::ScriptEventSourceHolder::GetSingleton();
        auto eventSourceSKSE = SKSE::GetActionEventSource();

        // WeaponEvents
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

        // SpellEvents
        spellEventHandler = std::make_unique<SpellEventHandler>(jsonLoader);

        RegisterMultipleEventHandlers(eventSource, spellEventHandler.get(), RE::TESEquipEvent {});
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
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
        spellEventHandler->SetupSpellTasks();

        RegisterMultipleEventHandlers(
            eventSourceSKSE, spellEventHandler.get(), SKSE::ActionEvent {});

        // ArmorEvents
        armorEventHandler = std::make_unique<ArmorEventHandler>(jsonLoader);

        RegisterMultipleEventHandlers(eventSource,
                                      armorEventHandler.get(),
                                      RE::TESContainerChangedEvent {},
                                      RE::TESEquipEvent {});

        // MiscItemEvents
        miscItemEventHandler = std::make_unique<MiscItemEventHandler>(jsonLoader);

        RegisterMultipleEventHandlers(eventSource,
                                      miscItemEventHandler.get(),
                                      RE::TESContainerChangedEvent {});
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
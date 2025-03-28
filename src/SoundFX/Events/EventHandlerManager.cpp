#include "EventHandlerManager.h"

namespace SoundFX {

    EventHandlerManager::EventHandlerManager(JSONLoader *jsonLoader) : jsonLoader(jsonLoader) {
    }

    void
        EventHandlerManager::InitializeEventHandlers() {

        const auto eventSource = RE::ScriptEventSourceHolder::GetSingleton();
        if (!eventSource) {
            spdlog::error("Failed to get ScriptEventSourceHolder singleton.");
            return;
        }

        const auto eventSourceSKSE = SKSE::GetActionEventSource();
        if (!eventSourceSKSE) {
            spdlog::error("Failed to get SKSE ActionEventSource.");
            return;
        }

        const auto ui = RE::UI::GetSingleton();
        if (!ui) {
            spdlog::error("Failed to get UI Singleton.");
            return;
        }

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

        RegisterMultipleEventHandlers(
            eventSource, miscItemEventHandler.get(), RE::TESContainerChangedEvent {});

        // QuestEvents
        questEventHandler = std::make_unique<QuestEventHandler>(jsonLoader);

        RegisterMultipleEventHandlers(
            eventSource, questEventHandler.get(), RE::TESQuestStageEvent {});

        // CellEvents
        cellEventHandler = std::make_unique<CellEventHandler>(jsonLoader);

        cellEventHandler->SetupCellTasks();

        // NpcInteractions
        npcInteractionEventHandler = std::make_unique<NpcInteractionEventHandler>(jsonLoader);
        npcInteractionEventHandler->InitializeOnlyAtTypeHandlers();
        npcInteractionEventHandler->SetupNpcInteractionTasks();

        RegisterMultipleEventHandlers(
            ui, npcInteractionEventHandler.get(), RE::MenuOpenCloseEvent {});

        // CombatEvents
        combatEventHandler = std::make_unique<CombatEventHandler>(jsonLoader);

        combatEventHandler->SetupCombatTasks();

        RegisterMultipleEventHandlers(eventSource, combatEventHandler.get(), RE::TESCombatEvent {});
    }

    RE::BSEventNotifyControl
        EventHandlerManager::ProcessMultipleEvents(
            const std::initializer_list<RE::BSEventNotifyControl> events) {
        for (const auto eventResult : events) {
            if (eventResult == RE::BSEventNotifyControl::kStop) {
                return RE::BSEventNotifyControl::kStop;
            }
        }
        return RE::BSEventNotifyControl::kContinue;
    }

}
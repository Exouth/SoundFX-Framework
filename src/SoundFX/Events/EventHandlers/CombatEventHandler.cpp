#include "CombatEventHandler.h"
#include "Events/EventHandlerManager.h"
#include "Sound/SoundManager.h"
#include "Utility.h"

namespace SoundFX {

    void
        CombatEventHandler::SetupCombatTasks() {
        StartCombatTask([this] { ProcessKillMoveCombatTask(); }, true);
        scheduler.Start(500);  // Run every 0.5 Second
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessEvent(const RE::TESCombatEvent *event,
                                         RE::BSTEventSource<RE::TESCombatEvent> *) {
        return EventHandlerManager::ProcessMultipleEvents({ProcessStartCombatEvent(event),
                                                           ProcessSearchCombatEvent(event),
                                                           ProcessStopCombatEvent(event),
                                                           ProcessDieCombatEvent(event),
                                                           ProcessFleeCombatEvent(event)});
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessStartCombatEvent(const RE::TESCombatEvent *event) const {

        if (event == nullptr || event->newState.any(RE::ACTOR_COMBAT_STATE::kNone)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!event->targetActor || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        // Currently Only Play when Player is involved, skip if Combat is between NPCs
        if (player->GetFormID() != event->targetActor->GetFormID()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &combats = jsonLoader->getItems("combats");
             const auto &combatEvents : combats | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Start"
                        && event->newState.any(RE::ACTOR_COMBAT_STATE::kCombat)) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(combatEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessSearchCombatEvent(const RE::TESCombatEvent *event) const {

        if (event == nullptr || event->newState.any(RE::ACTOR_COMBAT_STATE::kNone)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (!event->targetActor || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        // Currently Only Play when Player is involved, skip if Combat is between NPCs
        if (player->GetFormID() != event->targetActor->GetFormID()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &combats = jsonLoader->getItems("combats");
             const auto &combatEvents : combats | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Searching"
                        && event->newState.any(RE::ACTOR_COMBAT_STATE::kSearching)) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(combatEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessStopCombatEvent(const RE::TESCombatEvent *event) const {

        if (!event || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *eventActor = event->actor->As<RE::Actor>();
        if (!eventActor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (IsPlayerInvolvedInCombat(eventActor, player)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &combats = jsonLoader->getItems("combats");
             const auto &combatEvents : combats | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Stop"
                        && event->newState == RE::ACTOR_COMBAT_STATE::kNone) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(combatEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessDieCombatEvent(const RE::TESCombatEvent *event) const {

        if (!event || !event->actor || !event->actor->GetBaseObject()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *eventActor = event->actor->As<RE::Actor>();
        if (!eventActor) {
            return RE::BSEventNotifyControl::kContinue;
        }

        const float currentHP =
            eventActor->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
        if (currentHP > 0.0f) {
            if (!player->IsInKillMove()) {
                return RE::BSEventNotifyControl::kContinue;
            }
        }

        if (IsPlayerInvolvedInCombat(eventActor, player)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        for (const auto &combats = jsonLoader->getItems("combats");
             const auto &combatEvents : combats | std::views::values) {
            const auto resolvedFormID =
                GetFormIDFromEditorIDAndPluginName(combatEvents.editorID, combatEvents.pluginName);

            if (resolvedFormID == event->actor->GetBaseObject()->formID) {
                for (const auto &jsonEvent : combatEvents.events) {
                    if (jsonEvent.type == "Die"
                        && event->newState == RE::ACTOR_COMBAT_STATE::kNone) {
                        if (const float randomValue = GenerateRandomFloat();
                            randomValue <= jsonEvent.chance) {
                            SoundManager::PlaySound(combatEvents.name,
                                                    jsonEvent.type,
                                                    jsonEvent.soundEffect,
                                                    {8029.8643f, -69089.37f, 4815.15f},
                                                    1000.0f,
                                                    jsonEvent.volume,
                                                    jsonEvent.isAbsoluteVolume,
                                                    true);
                        }
                        return RE::BSEventNotifyControl::kContinue;
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    RE::BSEventNotifyControl
        CombatEventHandler::ProcessFleeCombatEvent(const RE::TESCombatEvent *event) const {

        if (!event || !event->targetActor || !event->actor || !event->actor->GetBaseObject()
            || !event->newState.any(RE::ACTOR_COMBAT_STATE::kCombat)) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *eventActor = event->actor->As<RE::Actor>();

        // Currently Only Play when Player is involved, skip if Combat is between NPCs
        if (const auto *player = RE::PlayerCharacter::GetSingleton();
            !eventActor || !player || player->GetFormID() != event->targetActor->GetFormID()) {
            return RE::BSEventNotifyControl::kContinue;
        }

        DelayExec(0.3f, [this, eventActor] {
            if (!eventActor || !IsActorFleeing(eventActor))
                return;

            for (const auto &combats = jsonLoader->getItems("combats");
                 const auto &combatEvents : combats | std::views::values) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    combatEvents.editorID, combatEvents.pluginName);

                if (resolvedFormID == eventActor->GetBaseObject()->formID) {
                    for (const auto &jsonEvent : combatEvents.events) {
                        if (jsonEvent.type == "Flee") {
                            if (const float randomValue = GenerateRandomFloat();
                                randomValue <= jsonEvent.chance) {
                                SoundManager::PlaySound(combatEvents.name,
                                                        jsonEvent.type,
                                                        jsonEvent.soundEffect,
                                                        {8029.8643f, -69089.37f, 4815.15f},
                                                        1000.0f,
                                                        jsonEvent.volume,
                                                        jsonEvent.isAbsoluteVolume,
                                                        true);
                            }
                            return;
                        }
                    }
                }
            }
        });

        return RE::BSEventNotifyControl::kContinue;
    }

    void
        CombatEventHandler::ProcessKillMoveCombatTask() const {

        auto       *player      = RE::PlayerCharacter::GetSingleton();
        const auto *playerActor = player->As<RE::Actor>();

        if (!player || !playerActor) {
            return;
        }

        static bool wasInKillMove = false;

        if (player->IsInKillMove()) {
            if (!wasInKillMove) {
                for (const auto &combats = jsonLoader->getItems("combats");
                     const auto &combatEvents : combats | std::views::values) {
                    const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                        combatEvents.editorID, combatEvents.pluginName);

                    RE::Actor *foundActor = GetCombatTargetByFormID(playerActor, resolvedFormID);

                    if (!foundActor || !foundActor->GetBaseObject()
                        || !foundActor->GetBaseObject()->formID) {
                        return;
                    }

                    if (resolvedFormID == foundActor->GetBaseObject()->formID) {
                        for (const auto &jsonEvent : combatEvents.events) {
                            if (jsonEvent.type == "KillMove") {
                                if (const float randomValue = GenerateRandomFloat();
                                    randomValue <= jsonEvent.chance) {
                                    wasInKillMove = true;
                                    SoundManager::PlaySound(combatEvents.name,
                                                            jsonEvent.type,
                                                            jsonEvent.soundEffect,
                                                            {8029.8643f, -69089.37f, 4815.15f},
                                                            1000.0f,
                                                            jsonEvent.volume,
                                                            jsonEvent.isAbsoluteVolume,
                                                            true);
                                }
                            }
                        }
                    }
                }
            }
            wasInKillMove = true;
        } else {
            wasInKillMove = false;
        }
    }

    bool
        CombatEventHandler::IsPlayerInvolvedInCombat(const RE::Actor           *eventActor,
                                                     const RE::PlayerCharacter *player) {
        if (auto *combatGroup = eventActor->GetCombatGroup()) {
            for (const auto &target : combatGroup->targets) {
                RE::NiPointer<RE::Actor> targetActorPtr = target.targetHandle.get();
                if (!targetActorPtr)
                    continue;

                const RE::Actor *targetActor = targetActorPtr.get();
                if (!targetActor)
                    continue;

                if (targetActor == player) {
                    return true;
                }
            }
        }
        return false;
    }

    bool
        CombatEventHandler::IsActorFleeing(const RE::Actor *eventActor) {
        const auto &runtimeData      = eventActor->GetActorRuntimeData();
        const auto *combatController = runtimeData.combatController;

        return combatController && combatController->state && combatController->state->isFleeing;
    }

    float
        CombatEventHandler::GetDistanceBetweenActors(const RE::Actor *actor1,
                                                     const RE::Actor *actor2) {
        if (!actor1 || !actor2)
            return FLT_MAX;

        const RE::NiPoint3 pos1 = actor1->GetPosition();
        const RE::NiPoint3 pos2 = actor2->GetPosition();

        return pos1.GetDistance(pos2);
    }

    std::vector<RE::Actor *>
        CombatEventHandler::GetCombatTargets(const RE::Actor *eventActor) {
        std::vector<RE::Actor *> targets;

        if (!eventActor) {
            return targets;
        }

        if (const auto *combatGroup = eventActor->GetCombatGroup()) {
            for (const auto &target : combatGroup->targets) {
                RE::NiPointer<RE::Actor> targetActorPtr = target.targetHandle.get();
                if (targetActorPtr) {
                    RE::Actor  *targetActor = targetActorPtr.get();
                    const float distance    = GetDistanceBetweenActors(eventActor, targetActor);
                    if (targetActor && distance <= MAX_DISTANCE) {
                        targets.push_back(targetActor);
                    }
                }
            }
        }

        return targets;
    }

    RE::Actor *
        CombatEventHandler::FindActorByFormID(const RE::Actor                *playerActor,
                                              const RE::FormID                formID,
                                              const std::vector<RE::Actor *> &actors) {
        RE::Actor *closestActor    = nullptr;
        float      closestDistance = FLT_MAX;

        for (auto *actor : actors) {
            if (actor && actor->GetBaseObject() && actor->GetBaseObject()->formID == formID) {
                const float distance = GetDistanceBetweenActors(playerActor, actor);
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closestActor    = actor;
                }
            }
        }
        return closestActor;
    }

    RE::Actor *
        CombatEventHandler::GetCombatTargetByFormID(const RE::Actor *eventActor,
                                                    const RE::FormID formID) {
        const auto targets = GetCombatTargets(eventActor);
        return FindActorByFormID(eventActor, formID, targets);
    }
}
#include "NpcInteractionEventHandler.h"

namespace SoundFX {

    std::unordered_map<std::string, NpcInteractionEventHandler::EventAction>
        NpcInteractionEventHandler::actionMap;

    void
        NpcInteractionEventHandler::InitializeOnlyAtTypeHandlers() {
        actionMap["All"] = [](const std::string &soundEffect,
                              const std::string &editorid,
                              const std::string &selectedDialogue) {
            PlayCustomSoundAsDescriptor(soundEffect);
        };
        actionMap["EditorID"] = [](const std::string &soundEffect,
                                   const std::string &editorid,
                                   const std::string &selectedDialogue) {
            if (editorid == selectedDialogue) {
                PlayCustomSoundAsDescriptor(soundEffect);
            }
        };
    }

    void
        NpcInteractionEventHandler::SetupNpcInteractionTasks() {
        StartNpcInteractionTask([this]() { ProcessDialogTopicTask(); }, true);
        scheduler.Start(100);  // Run every 0.1 Second
    }

    RE::BSEventNotifyControl
        NpcInteractionEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent *event,
                                                 RE::BSTEventSource<RE::MenuOpenCloseEvent> *) {
        return ProcessDialogOpenEvent(event);
    }

    // This is not a 100% logic for the dialog opener event, as it checks whether the NPC is
    // in the players field of vision and whether the dialog window is open.
    // So there is no direct “interaction logic” involved here
    RE::BSEventNotifyControl
        NpcInteractionEventHandler::ProcessDialogOpenEvent(const RE::MenuOpenCloseEvent *event) {

        if (!event) {
            return RE::BSEventNotifyControl::kContinue;
        }

        auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return RE::BSEventNotifyControl::kContinue;
        }

        if (event->menuName == RE::DialogueMenu::MENU_NAME) {
            if (event->opening) {
                if (player) {
                    float range    = 300.0f;
                    float fovAngle = 90.0f;

                    auto npcs = GetNpcsInPlayerFOV(player, range, fovAngle);

                    const auto &npcInteractions = jsonLoader.getItems("npcInteractions");
                    for (const auto &[npcInteractiontName, npcInteractionEvents] :
                         npcInteractions) {
                        const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                            npcInteractionEvents.editorID, npcInteractionEvents.pluginName);

                        if (npcs.find(resolvedFormID) != npcs.end()) {
                            for (const auto &jsonEvent : npcInteractionEvents.events) {
                                if (jsonEvent.type == "DialogOpen") {
                                    float randomValue = GenerateRandomFloat();
                                    if (randomValue <= jsonEvent.chance) {
                                        PlayCustomSoundAsDescriptor(jsonEvent.soundEffect);
                                    }
                                    return RE::BSEventNotifyControl::kContinue;
                                }
                            }
                        }
                    }
                }
            }
        }

        return RE::BSEventNotifyControl::kContinue;
    }

    void
        NpcInteractionEventHandler::ProcessDialogTopicTask() {

        auto *ui = RE::UI::GetSingleton();
        if (!ui) {
            return;
        }

        if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME)) {

            auto *menuTopicManager = RE::MenuTopicManager::GetSingleton();
            if (!menuTopicManager) {
                return;
            }

            auto  speakerHandle = menuTopicManager->speaker;
            auto *speaker       = speakerHandle.get().get();

            if (!speaker || !speaker->GetBaseObject()) {
                return;
            }

            const auto &npcInteractions = jsonLoader.getItems("npcInteractions");
            for (const auto &[npcInteractiontName, npcInteractionEvents] : npcInteractions) {
                const auto resolvedFormID = GetFormIDFromEditorIDAndPluginName(
                    npcInteractionEvents.editorID, npcInteractionEvents.pluginName);

                if (resolvedFormID == speaker->GetBaseObject()->formID) {
                    for (const auto &jsonEvent : npcInteractionEvents.events) {
                        if (jsonEvent.type == "DialogTopic") {

                            const std::string &onlyAtOriginal = jsonEvent.details.onlyAt.value();
                            const std::string  key =
                                (onlyAtOriginal != "All") ? "EditorID" : onlyAtOriginal;

                            if (actionMap.find(key) != actionMap.end()) {
                                float randomValue = GenerateRandomFloat();
                                if (randomValue <= jsonEvent.chance) {
                                    auto *selectedNode = menuTopicManager->currentTopicInfo;
                                    if (selectedNode) {
                                        auto *selectedDialogue = selectedNode->parentTopic;

                                        if (!selectedDialogue->GetFormEditorID()
                                            // Sometimes a nullptr check is not sufficient, as the
                                            // string might be initialized but empty. We need to
                                            // skip such cases.
                                            || std::strlen(selectedDialogue->GetFormEditorID()) == 0
                                            || lastParentTopic == nullptr) {
                                            lastParentTopic = selectedDialogue;
                                            return;
                                        }

                                        if (selectedDialogue->GetFormEditorID()
                                            != lastParentTopic->GetFormEditorID()) {
                                            lastParentTopic = selectedDialogue;

                                            actionMap[key](jsonEvent.soundEffect,
                                                           jsonEvent.details.onlyAt.value(),
                                                           selectedDialogue->GetFormEditorID());
                                        }
                                    }
                                }
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    RE::NiPoint3
        NpcInteractionEventHandler::GetActorForwardVector(RE::Actor *actor) {
        if (!actor) {
            return {0.0f, 0.0f, 1.0f};  // Fallback
        }

        auto node = actor->Get3D();
        if (!node) {
            return {0.0f, 0.0f, 1.0f};  // Fallback
        }

        const auto &worldRotate = node->world.rotate;
        return {
            worldRotate.entry[0][1],  // x
            worldRotate.entry[1][1],  // y
            worldRotate.entry[2][1]   // z
        };
    }

    std::unordered_map<RE::FormID, RE::Actor *>
        NpcInteractionEventHandler::GetNpcsInPlayerFOV(RE::Actor *player,
                                                       float      range,
                                                       float      fovAngle) {
        std::unordered_map<RE::FormID, RE::Actor *> npcsInRange;

        if (!player) {
            return npcsInRange;
        }

        auto *processLists = RE::ProcessLists::GetSingleton();
        if (!processLists) {
            return npcsInRange;
        }

        RE::NiPoint3 playerForward = GetActorForwardVector(player);

        for (auto &handle : processLists->highActorHandles) {
            auto actor = handle.get().get();
            if (!actor || actor->GetHandle() == player->GetHandle() || !actor->Is3DLoaded()) {
                continue;
            }

            RE::NiPoint3 directionToActor = actor->GetPosition() - player->GetPosition();

            float distance = directionToActor.Length();
            if (distance > range) {
                continue;
            }

            directionToActor.Unitize();

            float dotProduct = playerForward.Dot(directionToActor);
            float angle =
                std::acos(dotProduct) * (180.0f / std::numbers::pi_v<float>);  // Angle in degrees

            if (angle <= fovAngle / 2.0f) {
                auto baseform = actor->GetBaseObject();
                if (baseform) {
                    auto baseFormID         = baseform->formID;
                    npcsInRange[baseFormID] = actor;
                }
            }
        }

        return npcsInRange;
    }

}
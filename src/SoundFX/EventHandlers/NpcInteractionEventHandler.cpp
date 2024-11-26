#include "NpcInteractionEventHandler.h"
#include "JSONLoader.h"
#include "SoundUtil.h"
#include "Utility.h"

namespace SoundFX {

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
                                    float randomValue = static_cast<float>(rand()) / RAND_MAX;
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
                std::acos(dotProduct) * (180.0f / static_cast<float>(M_PI));  // Angle in degrees

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
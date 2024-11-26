#pragma once

#include "JSONLoader.h"
#include "TaskScheduler.h"

namespace SoundFX {

    class NpcInteractionEventHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
        JSONLoader   &jsonLoader;

      public:
        explicit NpcInteractionEventHandler(JSONLoader &loader) : jsonLoader(loader) {
        }

        RE::BSEventNotifyControl
            ProcessEvent(const RE::MenuOpenCloseEvent *event,
                         RE::BSTEventSource<RE::MenuOpenCloseEvent> *);

      private:
        RE::BSEventNotifyControl
            ProcessDialogOpenEvent(const RE::MenuOpenCloseEvent *event);

        RE::NiPoint3
            GetActorForwardVector(RE::Actor *actor);

        std::unordered_map<RE::FormID, RE::Actor *>
            GetNpcsInPlayerFOV(RE::Actor *player, float range, float fovAngle);
    };
}
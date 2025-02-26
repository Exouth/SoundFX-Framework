#pragma once

#include "BaseHook.h"

namespace SoundFX {
    class InputEventHook : public BaseHook {
      public:
        void
            Install() override;

      private:
        static void
            DispatchInputEvent(RE::BSTEventSource<RE::InputEvent *> *dispatcher,
                               RE::InputEvent                      **event);
        static inline REL::Relocation<decltype(&DispatchInputEvent)> _DispatchInputEvent;
    };
}
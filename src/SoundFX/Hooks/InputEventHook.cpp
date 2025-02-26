#include "InputEventHook.h"
#include "ImGui/Core/ImGuiManager.h"
#include "ImGui/Core/InputListener.h"

namespace SoundFX {
    void
        InputEventHook::Install() {
        REL::Relocation<uintptr_t> hook {RELOCATION_ID(67315, 68617)};
        _DispatchInputEvent = SKSE::GetTrampoline().write_call<5>(
            hook.address() + REL::VariantOffset(0x7B, 0x7B, 0).offset(), DispatchInputEvent);
    }

    void
        InputEventHook::DispatchInputEvent(RE::BSTEventSource<RE::InputEvent *> *dispatcher,
                                           RE::InputEvent                      **event) {
        static RE::InputEvent *dummy[] = {nullptr};

        if (!event) {
            _DispatchInputEvent(dispatcher, event);
            return;
        }

        InputListener::GetSingleton()->ProcessEvent(event);

        if (ImGuiManager::IsUIVisible()) {
            _DispatchInputEvent(dispatcher, dummy);
        } else {
            _DispatchInputEvent(dispatcher, event);
        }
    }
}
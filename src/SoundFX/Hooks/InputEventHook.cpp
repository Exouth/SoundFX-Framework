#include "InputEventHook.h"
#include "ImGui/Core/ImGuiManager.h"
#include "ImGui/Core/InputListener.h"

namespace SoundFX {
    void
        InputEventHook::Install() {
        const REL::Relocation hook {RELOCATION_ID(67315, 68617)};
        s_DispatchInputEvent = SKSE::GetTrampoline().write_call<5>(
            hook.address() + REL::VariantOffset(0x7B, 0x7B, 0).offset(), DispatchInputEvent);
    }

    void
        InputEventHook::DispatchInputEvent(RE::BSTEventSource<RE::InputEvent *> *dispatcher,
                                           RE::InputEvent                      **event) {
        static RE::InputEvent *dummy[] = {nullptr};

        if (!event) {
            s_DispatchInputEvent(dispatcher, event);
            return;
        }

        if (const auto *listener = InputListener::GetSingleton()) {
            listener->ProcessEvent(event);
        }

        if (ImGuiManager::IsUIVisible()) {
            s_DispatchInputEvent(dispatcher, dummy);
        } else {
            s_DispatchInputEvent(dispatcher, event);
        }
    }
}
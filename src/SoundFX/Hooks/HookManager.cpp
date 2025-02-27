#include "HookManager.h"
#include "InputEventHook.h"
#include "RendererHook.h"

namespace SoundFX {
    HookManager &
        HookManager::GetInstance() {
        static HookManager instance;
        return instance;
    }

    void
        HookManager::InstallHooks() {
        RegisterHook<RendererHook>();
        RegisterHook<InputEventHook>();

        for (const auto &hook : hooks) {
            hook->Install();
        }
    }
}
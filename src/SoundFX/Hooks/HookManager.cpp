#include "HookManager.h"
#include "InputEventHook.h"
#include "VTableHook.h"

namespace SoundFX {
    HookManager &
        HookManager::GetInstance() {
        static HookManager instance;
        return instance;
    }

    void
        HookManager::InstallHooks() {
        RegisterHook<VTableHook>();
        RegisterHook<InputEventHook>();

        for (const auto &hook : hooks) {
            hook->Install();
        }
    }
}
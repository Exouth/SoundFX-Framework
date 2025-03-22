#include "HookManager.h"
#include "Config/ConfigManager.h"
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

        const bool debugUI =
            Config::ConfigManager::GetInstance().GetValue<bool>("GeneralSettings", "DebugUI", true);
        if (!debugUI) {
            spdlog::warn("DebugUI disabled via INI setting.");
            return;
        }

        RegisterHook<VTableHook>();
        RegisterHook<InputEventHook>();

        for (const auto &hook : hooks) {
            hook->Install();
        }
    }
}
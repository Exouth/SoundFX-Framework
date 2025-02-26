#pragma once

#include "BaseHook.h"

namespace SoundFX {
    class HookManager {
      public:
        static HookManager &
            GetInstance();

        void
            InstallHooks();  // Installs all hooks

        template <typename HookType, typename... Args>
        void
            RegisterHook(Args &&...args) {
            hooks.emplace_back(std::make_unique<HookType>(std::forward<Args>(args)...));
        }

      private:
        HookManager()  = default;
        ~HookManager() = default;

        HookManager(const HookManager &) = delete;
        HookManager &
            operator=(const HookManager &) = delete;

        std::vector<std::unique_ptr<BaseHook>> hooks;
    };
}
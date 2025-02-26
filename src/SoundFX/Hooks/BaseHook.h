#pragma once

namespace SoundFX {
    class BaseHook {
      public:
        virtual void
            Install()       = 0;  // Must be implemented by all hooks
        virtual ~BaseHook() = default;

        BaseHook(const BaseHook &) = delete;
        BaseHook &
            operator=(const BaseHook &) = delete;
        BaseHook(BaseHook &&)           = delete;
        BaseHook &
            operator=(BaseHook &&) = delete;

      protected:
        BaseHook() = default;
    };
}
#pragma once

namespace SoundFX {
    class BaseHook {
      public:
        virtual void
            Install()       = 0;  // Must be implemented by all hooks
        virtual ~BaseHook() = default;
    };
}
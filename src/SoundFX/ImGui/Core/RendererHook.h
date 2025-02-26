#pragma once
#include "RE/B/BSRenderManager.h"

namespace SoundFX {
    class RendererHook {
      public:
        static void
            Install();
        static void
            OnRender();
        static void
            DXGIPresentHook(std::uint32_t a_p1);
    };
}
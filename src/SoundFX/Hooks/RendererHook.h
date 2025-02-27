#pragma once

#include "BaseHook.h"
#include "RE/B/BSRenderManager.h"

namespace SoundFX {
    class RendererHook final : public BaseHook {
      public:
        void
            Install() override;

      private:
        static void
            DXGIPresentHook(std::uint32_t a_p1);
        static void
            OnRender();

        static inline ID3D11Device        *g_D3D11Device  = nullptr;
        static inline ID3D11DeviceContext *g_D3D11Context = nullptr;
        static inline bool                 g_Initialized  = false;
    };
}
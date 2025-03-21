#pragma once

#include "BaseHook.h"
#include <d3d11.h>
#include <wrl/client.h>

namespace SoundFX {
    class VTableHook final : public BaseHook {
      public:
        void
            Install() override;

      private:
        static HRESULT __stdcall hkPresent(IDXGISwapChain *pSwapChain,
                                           UINT            SyncInterval,
                                           UINT            Flags);
        static bool
            InitializeResources(IDXGISwapChain *pSwapChain);
        static void
            HookVTable(void **vTable);
        static void **
            GetSwapChainVTable();

        static inline Microsoft::WRL::ComPtr<ID3D11Device>           device           = nullptr;
        static inline Microsoft::WRL::ComPtr<ID3D11DeviceContext>    context          = nullptr;
        static inline Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView = nullptr;
        static inline HRESULT(__stdcall *originalPresent)(IDXGISwapChain *pSwapChain,
                                                          UINT            SyncInterval,
                                                          UINT            Flags)                 = nullptr;
    };
}
#include "RendererHook.h"
#include "ImGuiManager.h"

namespace {
    ID3D11Device        *g_D3D11Device  = nullptr;
    ID3D11DeviceContext *g_D3D11Context = nullptr;
    bool                 g_Initialized  = false;
}

namespace SoundFX {
    void
        RendererHook::OnRender() {
        if (!g_Initialized)
            return;
        ImGuiManager::Render();
    }

    void
        RendererHook::DXGIPresentHook(std::uint32_t a_p1) {
        const auto renderer = RE::BSGraphics::Renderer::GetSingleton();
        if (!renderer)
            return;

        const auto renderData = renderer->GetRendererData();
        g_D3D11Device         = reinterpret_cast<ID3D11Device *>(renderData->forwarder);
        g_D3D11Context        = reinterpret_cast<ID3D11DeviceContext *>(renderData->context);

        if (!g_D3D11Device || !g_D3D11Context)
            return;

        if (!g_Initialized) {
            ImGuiManager::Initialize();
            g_Initialized = true;
        }

        OnRender();
    }

    void
        RendererHook::Install() {
        const REL::Relocation hook {REL::RelocationID(75461, 77246)};
        auto                 &trampoline = SKSE::GetTrampoline();
        trampoline.write_call<5>(hook.address() + REL::VariantOffset(0x9, 0x9, 0x15).offset(),
                                 reinterpret_cast<uintptr_t>(&DXGIPresentHook));
    }
}
#include "VTableHook.h"
#include "ImGui/Core/ImGuiManager.h"
#include <RE/R/Renderer.h>
#include <d3dcompiler.h>
#include <dxgi.h>

#pragma comment(lib, "d3dcompiler.lib")

namespace SoundFX {
    void
        VTableHook::Install() {
        if (VTableEntry *vTable = GetSwapChainVTable()) {
            HookVTable(vTable);
        }
    }

    HRESULT
    __stdcall VTableHook::hkPresent(IDXGISwapChain *pSwapChain, UINT SyncInterval, UINT Flags) {
        static bool initialized = false;

        if (!initialized) {
            if (!InitializeResources(pSwapChain)) {
                return originalPresent(pSwapChain, SyncInterval, Flags);
            }
            initialized = true;
        }

        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backBuffer))) {
            spdlog::error("Failed to get BackBuffer");
            return originalPresent(pSwapChain, SyncInterval, Flags);
        }

        if (FAILED(device->CreateRenderTargetView(backBuffer.Get(), nullptr, &renderTargetView))) {
            spdlog::error("Failed to create RenderTargetView");
            return originalPresent(pSwapChain, SyncInterval, Flags);
        }
        context->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);

        ImGuiManager::Render();

        return originalPresent(pSwapChain, SyncInterval, Flags);
    }

    bool
        VTableHook::InitializeResources(IDXGISwapChain *pSwapChain) {
        if (FAILED(pSwapChain->GetDevice(__uuidof(ID3D11Device), &device))) {
            spdlog::error("Failed to get D3D11Device");
            return false;
        }
        device->GetImmediateContext(&context);

        if (const auto renderer = RE::BSGraphics::Renderer::GetSingleton(); !renderer) {
            spdlog::error("Renderer::GetSingleton() returned null");
            return false;
        }
        const RE::BSGraphics::RendererWindow *rendererWindow =
            RE::BSGraphics::Renderer::GetCurrentRenderWindow();
        const auto hwnd = reinterpret_cast<HWND>(rendererWindow->hWnd);
        ImGuiManager::Initialize(hwnd, device.Get(), context.Get());

        spdlog::info("Resources initialized successfully");
        return true;
    }

    void
        VTableHook::HookVTable(VTableEntry *vTable) {
        spdlog::info("Attempting to hook Present using VTable");

        originalPresent = vTable[8];
        DWORD oldProtect;
        if (VirtualProtect(&vTable[8], sizeof(void *), PAGE_EXECUTE_READWRITE, &oldProtect)) {
            vTable[8] = &hkPresent;
            VirtualProtect(&vTable[8], sizeof(void *), oldProtect, &oldProtect);
            spdlog::info("VTable hook installed for Present");
        } else {
            spdlog::error("Failed to change VTable protection");
        }
    }

    VTableEntry *
        VTableHook::GetSwapChainVTable() {
        const auto renderer = RE::BSGraphics::Renderer::GetSingleton();
        if (!renderer) {
            spdlog::error("Renderer::GetSingleton() returned null");
            return nullptr;
        }

        for (const auto &window : renderer->data.renderWindows) {
            if (window.swapChain) {
                VTableEntry *vTable = *reinterpret_cast<VTableEntry **>(window.swapChain);
                return vTable;
            }
        }

        return nullptr;
    }
}
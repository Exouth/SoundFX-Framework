#include "InputListener.h"

#define IM_VK_KEYPAD_ENTER (VK_RETURN + 256)

namespace {
    ImGuiKey
        VirtualKeyToImGuiKey(const WPARAM wParam) {
        switch (wParam) {
        case VK_TAB: return ImGuiKey_Tab;
        case VK_LEFT: return ImGuiKey_LeftArrow;
        case VK_RIGHT: return ImGuiKey_RightArrow;
        case VK_UP: return ImGuiKey_UpArrow;
        case VK_DOWN: return ImGuiKey_DownArrow;
        case VK_RETURN: return ImGuiKey_Enter;
        case VK_ESCAPE: return ImGuiKey_Escape;
        case VK_SPACE: return ImGuiKey_Space;
        case VK_BACK: return ImGuiKey_Backspace;
        case VK_DELETE: return ImGuiKey_Delete;
        default:
            return (wParam >= 'A' && wParam <= 'Z')
                     ? static_cast<ImGuiKey>(ImGuiKey_A + (wParam - 'A'))
                     : ImGuiKey_None;
        }
    }
}
namespace SoundFX {
    InputListener *
        InputListener::GetSingleton() {
        static InputListener instance;
        return &instance;
    }

    void
        InputListener::ProcessEvent(RE::InputEvent **event) {
        if (!event || !*event) {
            return;
        }

        auto &io = ImGui::GetIO();

        for (auto e = *event; e; e = e->next) {
            if (e->eventType != RE::INPUT_EVENT_TYPE::kButton) {
                continue;
            }

            const auto *button = static_cast<RE::ButtonEvent *>(e);
            if (!button || (!button->IsPressed() && button->IsDown())) {
                continue;
            }

            // I dont really know at the Moment whether there are very large values returned (Maybe
            // Look more into it later)
            const int scanCode = std::clamp(static_cast<int>(button->GetIDCode()), 0, 255);
            switch (button->device.get()) {
            case RE::INPUT_DEVICE::kMouse:
                if (scanCode <= 5) {
                    io.AddMouseButtonEvent(scanCode, button->IsPressed());
                }
                break;
            case RE::INPUT_DEVICE::kKeyboard:
                io.AddKeyEvent(VirtualKeyToImGuiKey(scanCode), button->IsPressed());
                break;
            default: break;
            }
        }
    }
}
#include "InputListener.h"
#include "ImGui/Core/ImGuiManager.h"

namespace {
    ImGuiKey constexpr VirtualKeyToImGuiKey(const WPARAM wParam) {
        switch (wParam) {
        case 200: return ImGuiKey_UpArrow;
        case 208: return ImGuiKey_DownArrow;
        case 203: return ImGuiKey_LeftArrow;
        case 205: return ImGuiKey_RightArrow;
        case 199: return ImGuiKey_Home;
        case 207: return ImGuiKey_End;
        case 201: return ImGuiKey_PageUp;
        case 209: return ImGuiKey_PageDown;
        case 211: return ImGuiKey_Delete;
        case 57: return ImGuiKey_Space;
        case 14: return ImGuiKey_Backspace;
        default: return ImGuiKey_None;
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
            if (e->eventType == RE::INPUT_EVENT_TYPE::kButton) {
                const auto *button = static_cast<RE::ButtonEvent *>(e);
                if (!button) {
                    continue;
                }

                // I dont really know at the Moment whether there are very large values returned
                // (Maybe Look more into it later)
                const int scanCode = std::clamp(static_cast<int>(button->GetIDCode()), 0, 255);

                const auto buttonDevice = button->device.get();

                // Deactive UI for ESC
                if (buttonDevice == RE::INPUT_DEVICE::kKeyboard && scanCode == 1
                    && ImGuiManager::IsUIVisible()) {
                    ImGuiManager::ToggleUI();
                    return;
                }

                switch (buttonDevice) {
                case RE::INPUT_DEVICE::kMouse:
                    if (scanCode <= 5) {
                        io.AddMouseButtonEvent(scanCode, button->IsPressed());
                    }
                    break;

                case RE::INPUT_DEVICE::kKeyboard:
                    {
                        io.AddKeyEvent(VirtualKeyToImGuiKey(scanCode), button->IsPressed());
                        break;
                    }

                default: break;
                }
            } else if (e->eventType == RE::INPUT_EVENT_TYPE::kChar) {
                if (const auto *charEvent = static_cast<RE::CharEvent *>(e)) {
                    io.AddInputCharacter(charEvent->keycode);
                }
            }
        }
    }
}
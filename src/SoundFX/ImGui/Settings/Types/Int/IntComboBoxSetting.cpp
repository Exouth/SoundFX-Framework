#include "ImGui/Settings/Types/Int/IntComboBoxSetting.h"

namespace SoundFX {

    IntComboBoxSetting::IntComboBoxSetting(std::string              settingName,
                                           std::string              paramIniKey,
                                           int                      initialDefaultValue,
                                           std::vector<int>         options,
                                           std::function<void(int)> onValueChange,
                                           std::string              desc) :
        IntSetting(std::move(settingName),
                   std::move(paramIniKey),
                   initialDefaultValue,
                   std::move(onValueChange),
                   std::move(desc)),
        options(std::move(options)) {
    }

    void
        IntComboBoxSetting::Render() {
        ImGui::Text("%s", GetName().c_str());

        int currentValue = GetValue();
        if (ImGui::BeginCombo("", std::to_string(currentValue).c_str())) {
            for (const auto &option : options) {
                const bool isSelected = currentValue == option;
                if (ImGui::Selectable(std::to_string(option).c_str(), isSelected)) {
                    SetValue(option);
                    Save();
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (GetValue() != GetDefaultValue()) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + GetName()).c_str())) {
                Reset();
            }
        }
    }
}
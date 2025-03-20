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
        ImGui::Text("%s", name.c_str());

        if (ImGui::BeginCombo("", std::to_string(value).c_str())) {
            for (const auto &option : options) {
                const bool isSelected = value == option;
                if (ImGui::Selectable(std::to_string(option).c_str(), isSelected)) {
                    value = option;
                    onChange(value);
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

        if (value != defaultValue) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + name).c_str())) {
                Reset();
            }
        }
    }
}
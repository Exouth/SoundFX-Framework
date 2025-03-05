#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"

namespace SoundFX {

    BoolCheckboxSetting::BoolCheckboxSetting(std::string               settingName,
                                             bool                      initialDefaultValue,
                                             std::function<void(bool)> onValueChange,
                                             std::string               desc) :
        BoolSetting(std::move(settingName),
                    initialDefaultValue,
                    std::move(onValueChange),
                    std::move(desc)) {
    }

    void
        BoolCheckboxSetting::Render() {
        if (ImGui::Checkbox(name.c_str(), &value)) {
            onChange(value);
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (value != defaultValue) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + name).c_str())) {
                value = defaultValue;
                onChange(value);
            }
        }
    }
}
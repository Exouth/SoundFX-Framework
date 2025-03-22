#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"

namespace SoundFX {

    BoolCheckboxSetting::BoolCheckboxSetting(std::string               settingName,
                                             std::string               paramIniKey,
                                             bool                      initialDefaultValue,
                                             std::function<void(bool)> onValueChange,
                                             std::string               desc) :
        BoolSetting(std::move(settingName),
                    std::move(paramIniKey),
                    initialDefaultValue,
                    std::move(onValueChange),
                    std::move(desc)) {
    }

    void
        BoolCheckboxSetting::Render() {
        bool currentValue = GetValue();
        if (ImGui::Checkbox(GetNameRef().c_str(), &currentValue)) {
            SetValue(currentValue);
            Save();
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (GetValue() != GetDefaultValue()) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + GetNameRef()).c_str())) {
                Reset();
            }
        }
    }
}
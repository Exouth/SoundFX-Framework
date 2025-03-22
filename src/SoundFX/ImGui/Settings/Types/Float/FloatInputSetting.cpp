#include "ImGui/Settings/Types/Float/FloatInputSetting.h"

namespace SoundFX {

    FloatInputSetting::FloatInputSetting(std::string                settingName,
                                         std::string                paramIniKey,
                                         float                      initialDefaultValue,
                                         std::function<void(float)> onValueChange,
                                         std::string                desc) :
        FloatSetting(std::move(settingName),
                     std::move(paramIniKey),
                     initialDefaultValue,
                     std::move(onValueChange),
                     std::move(desc)) {
    }

    void
        FloatInputSetting::Render() {
        float currentValue = GetValue();
        if (ImGui::InputFloat(GetNameRef().c_str(), &currentValue)) {
            SetValue(currentValue);
            Save();
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (constexpr float epsilon = 0.01f; std::fabs(GetValue() - GetDefaultValue()) > epsilon) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + GetNameRef()).c_str())) {
                Reset();
            }
        }
    }
}
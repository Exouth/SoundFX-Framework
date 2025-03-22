#include "ImGui/Settings/Types/Float/FloatSliderSetting.h"

namespace SoundFX {

    FloatSliderSetting::FloatSliderSetting(std::string                settingName,
                                           std::string                paramIniKey,
                                           float                      initialDefaultValue,
                                           float                      minRangeValue,
                                           float                      maxRangeValue,
                                           std::function<void(float)> onValueChange,
                                           std::string                desc) :
        FloatSetting(std::move(settingName),
                     std::move(paramIniKey),
                     initialDefaultValue,
                     std::move(onValueChange),
                     std::move(desc)),
        minValue(minRangeValue),
        maxValue(maxRangeValue) {
    }

    void
        FloatSliderSetting::Render() {
        float currentValue = GetValue();
        ImGui::Text("%s", GetNameRef().c_str());

        if (ImGui::SliderFloat(("##" + GetNameRef()).c_str(), &currentValue, minValue, maxValue)) {
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
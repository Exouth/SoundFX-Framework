#include "ImGui/Settings/Types/Float/FloatSliderSetting.h"

namespace SoundFX {

    FloatSliderSetting::FloatSliderSetting(std::string                settingName,
                                           float                      initialDefaultValue,
                                           float                      minRangeValue,
                                           float                      maxRangeValue,
                                           std::function<void(float)> onValueChange) :
        FloatSetting(std::move(settingName), initialDefaultValue, std::move(onValueChange)),
        minValue(minRangeValue),
        maxValue(maxRangeValue) {
    }

    void
        FloatSliderSetting::Render() {
        if (ImGui::SliderFloat(name.c_str(), &value, minValue, maxValue)) {
            onChange(value);
        }
        if (constexpr float epsilon = 0.01f; std::fabs(value - defaultValue) > epsilon) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + name).c_str())) {
                value = defaultValue;
                onChange(value);
            }
        }
    }
}
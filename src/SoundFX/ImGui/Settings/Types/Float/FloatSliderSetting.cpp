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
        ImGui::Text("%s", name.c_str());

        if (ImGui::SliderFloat(("##" + name).c_str(), &value, minValue, maxValue)) {
            onChange(value);
            Save();
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (constexpr float epsilon = 0.01f; std::fabs(value - defaultValue) > epsilon) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + name).c_str())) {
                Reset();
            }
        }
    }
}
#include "ImGui/Settings/Types/Int/IntSliderSetting.h"

namespace SoundFX {

    IntSliderSetting::IntSliderSetting(std::string              settingName,
                                       std::string              paramIniKey,
                                       int                      initialDefaultValue,
                                       int                      minRangeValue,
                                       int                      maxRangeValue,
                                       std::function<void(int)> onValueChange,
                                       std::string              desc) :
        IntSetting(std::move(settingName),
                   std::move(paramIniKey),
                   initialDefaultValue,
                   std::move(onValueChange),
                   std::move(desc)),
        minValue(minRangeValue),
        maxValue(maxRangeValue) {
    }

    void
        IntSliderSetting::Render() {
        ImGui::Text("%s", name.c_str());

        if (ImGui::SliderInt(("##" + name).c_str(), &value, minValue, maxValue)) {
            onChange(value);
            Save();
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
#include "ImGui/Settings/Types/Float/FloatInputSetting.h"

namespace SoundFX {

    FloatInputSetting::FloatInputSetting(std::string                settingName,
                                         float                      initialDefaultValue,
                                         std::function<void(float)> onValueChange,
                                         std::string                desc) :
        FloatSetting(std::move(settingName),
                     initialDefaultValue,
                     std::move(onValueChange),
                     std::move(desc)) {
    }

    void
        FloatInputSetting::Render() {
        if (ImGui::InputFloat(name.c_str(), &value)) {
            onChange(value);
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
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
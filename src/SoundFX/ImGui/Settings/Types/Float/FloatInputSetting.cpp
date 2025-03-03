#include "ImGui/Settings/Types/Float/FloatInputSetting.h"

namespace SoundFX {

    FloatInputSetting::FloatInputSetting(std::string                settingName,
                                         float                      initialDefaultValue,
                                         std::function<void(float)> onValueChange) :
        FloatSetting(std::move(settingName), initialDefaultValue, std::move(onValueChange)) {
    }

    void
        FloatInputSetting::Render() {
        if (ImGui::InputFloat(name.c_str(), &value)) {
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
#include "ImGui/Settings/Types/Bool/BoolCheckboxSetting.h"

namespace SoundFX {

    BoolCheckboxSetting::BoolCheckboxSetting(const std::string               &settingName,
                                             bool                             initialDefaultValue,
                                             const std::function<void(bool)> &onValueChange) :
        BoolSetting(settingName, initialDefaultValue, onValueChange) {
    }

    void
        BoolCheckboxSetting::Render() {
        if (ImGui::Checkbox(name.c_str(), &value)) {
            onChange(value);
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
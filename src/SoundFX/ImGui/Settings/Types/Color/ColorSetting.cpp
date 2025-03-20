#include "ImGui/Settings/Types/Color/ColorSetting.h"

namespace SoundFX {
    ColorSetting::ColorSetting(std::string                 settingName,
                               std::string                 iniKey,
                               const ImVec4               &defaultColor,
                               std::function<void(ImVec4)> onValueChange,
                               std::string                 desc) :
        BaseSetting(std::move(desc)),
        name(std::move(settingName)),
        iniKey(std::move(iniKey)),
        value(defaultColor),
        defaultValue(defaultColor),
        onChange(std::move(onValueChange)) {
        Load();
    }

    void
        ColorSetting::Load() {
        value = config.GetValue<ImVec4>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        ColorSetting::Save() const {
        config.SetValue<ImVec4>(INI_SECTION, iniKey.c_str(), value);
        config.Save();
    }

    void
        ColorSetting::Reset() {
        value = defaultValue;
        onChange(value);
        Save();
    }

    std::string
        ColorSetting::GetName() const {
        return name;
    }

    void
        ColorSetting::Render() {
        ImGui::Text("%s", name.c_str());

        if (ImGui::ColorEdit4(("##" + name).c_str(), reinterpret_cast<float *>(&value))) {
            onChange(value);
            Save();
        }

        ImGui::SameLine();
        ImGui::Text(ICON_FA_QUESTION);
        if (ImGui::IsItemHovered()) {
            RenderTooltip();
        }

        if (constexpr float EPSILON = 0.0001f; std::fabs(value.x - defaultValue.x) > EPSILON
                                               || std::fabs(value.y - defaultValue.y) > EPSILON
                                               || std::fabs(value.z - defaultValue.z) > EPSILON
                                               || std::fabs(value.w - defaultValue.w) > EPSILON) {
            ImGui::SameLine();
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + name).c_str())) {
                Reset();
            }
        }
    }
}
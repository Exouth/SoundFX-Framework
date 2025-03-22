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
        value = GetConfigManager().GetValue<ImVec4>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        ColorSetting::Save() const {
        GetConfigManager().SetValue<ImVec4>(INI_SECTION, iniKey.c_str(), value);
        GetConfigManager().Save();
    }

    void
        ColorSetting::Reset() {
        SetValue(defaultValue);
        Save();
    }

    std::string
        ColorSetting::GetName() const {
        return name;
    }

    void
        ColorSetting::Render() {
        ImGui::Text("%s", GetNameRef().c_str());

        ImVec4 currentValue = GetValue();
        if (ImGui::ColorEdit4(("##" + GetNameRef()).c_str(),
                              reinterpret_cast<float *>(&currentValue))) {
            SetValue(currentValue);
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
            if (ImGui::Button((std::string(ICON_FA_UNDO) + "##" + GetNameRef()).c_str())) {
                Reset();
            }
        }
    }
}
#include "ImGui/Settings/Types/Float/FloatSetting.h"

namespace SoundFX {

    FloatSetting::FloatSetting(std::string                name,
                               std::string                iniKey,
                               float                      defaultValue,
                               std::function<void(float)> onChange,
                               std::string                desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        iniKey(std::move(iniKey)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
        Load();
    }

    void
        FloatSetting::Load() {
        value = GetConfigManager().GetValue<float>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        FloatSetting::Save() const {
        GetConfigManager().SetValue<float>(INI_SECTION, iniKey.c_str(), value);
        GetConfigManager().Save();
    }

    void
        FloatSetting::Reset() {
        SetValue(defaultValue);
        Save();
    }

    std::string
        FloatSetting::GetName() const {
        return name;
    }
}
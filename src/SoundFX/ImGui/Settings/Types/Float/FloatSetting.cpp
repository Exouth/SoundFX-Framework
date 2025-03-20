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
        value = config.GetValue<float>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        FloatSetting::Save() const {
        config.SetValue<float>(INI_SECTION, iniKey.c_str(), value);
        config.Save();
    }

    void
        FloatSetting::Reset() {
        value = defaultValue;
        onChange(value);
        Save();
    }

    std::string
        FloatSetting::GetName() const {
        return name;
    }
}
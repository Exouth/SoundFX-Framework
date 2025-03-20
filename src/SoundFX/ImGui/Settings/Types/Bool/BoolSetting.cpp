#include "ImGui/Settings/Types/Bool/BoolSetting.h"

namespace SoundFX {

    BoolSetting::BoolSetting(std::string               name,
                             std::string               iniKey,
                             bool                      defaultValue,
                             std::function<void(bool)> onChange,
                             std::string               desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        iniKey(std::move(iniKey)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
        Load();
    }

    void
        BoolSetting::Load() {
        value = config.GetValue<bool>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        BoolSetting::Save() const {
        config.SetValue<bool>(INI_SECTION, iniKey.c_str(), value);
        config.Save();
    }

    void
        BoolSetting::Reset() {
        value = defaultValue;
        onChange(value);
        Save();
    }

    std::string
        BoolSetting::GetName() const {
        return name;
    }
}
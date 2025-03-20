#include "ImGui/Settings/Types/Int/IntSetting.h"

namespace SoundFX {

    IntSetting::IntSetting(std::string              name,
                           std::string              iniKey,
                           int                      defaultValue,
                           std::function<void(int)> onChange,
                           std::string              desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        iniKey(std::move(iniKey)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
        Load();
    }

    void
        IntSetting::Load() {
        value = config.GetValue<int>(INI_SECTION, iniKey.c_str(), defaultValue);
        onChange(value);
    }

    void
        IntSetting::Save() const {
        config.SetValue<int>(INI_SECTION, iniKey.c_str(), value);
        config.Save();
    }

    void
        IntSetting::Reset() {
        value = defaultValue;
        onChange(value);
        Save();
    }

    std::string
        IntSetting::GetName() const {
        return name;
    }
}
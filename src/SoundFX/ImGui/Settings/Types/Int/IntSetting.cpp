#include "ImGui/Settings/Types/Int/IntSetting.h"

namespace SoundFX {

    IntSetting::IntSetting(std::string              name,
                           int                      defaultValue,
                           std::function<void(int)> onChange,
                           std::string              desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
    }

    void
        IntSetting::Reset() {
        value = defaultValue;
        onChange(value);
    }

    std::string
        IntSetting::GetName() const {
        return name;
    }
}
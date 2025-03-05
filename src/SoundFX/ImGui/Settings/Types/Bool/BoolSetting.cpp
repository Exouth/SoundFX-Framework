#include "ImGui/Settings/Types/Bool/BoolSetting.h"

namespace SoundFX {

    BoolSetting::BoolSetting(std::string               name,
                             bool                      defaultValue,
                             std::function<void(bool)> onChange,
                             std::string               desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
    }

    void
        BoolSetting::Reset() {
        value = defaultValue;
        onChange(value);
    }

    std::string
        BoolSetting::GetName() const {
        return name;
    }
}
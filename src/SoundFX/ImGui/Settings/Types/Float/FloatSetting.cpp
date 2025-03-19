#include "ImGui/Settings/Types/Float/FloatSetting.h"

namespace SoundFX {

    FloatSetting::FloatSetting(std::string                name,
                               float                      defaultValue,
                               std::function<void(float)> onChange,
                               std::string                desc) :
        BaseSetting(std::move(desc)),
        name(std::move(name)),
        value(defaultValue),
        defaultValue(defaultValue),
        onChange(std::move(onChange)) {
    }

    void
        FloatSetting::Reset() {
        value = defaultValue;
        onChange(value);
    }

    std::string
        FloatSetting::GetName() const {
        return name;
    }
}
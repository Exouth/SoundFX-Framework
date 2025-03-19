#pragma once

#include "ImGui/Settings/Types/Float/FloatSetting.h"

namespace SoundFX {
    class FloatSliderSetting final : public FloatSetting {
      public:
        FloatSliderSetting(std::string                settingName,
                           float                      initialDefaultValue,
                           float                      minRangeValue,
                           float                      maxRangeValue,
                           std::function<void(float)> onValueChange,
                           std::string                desc = "");
        void
            Render() override;

      private:
        float minValue;
        float maxValue;
    };
}
#pragma once

#include "ImGui/Settings/Types/Int/IntSetting.h"

namespace SoundFX {
    class IntSliderSetting final : public IntSetting {
      public:
        IntSliderSetting(std::string              settingName,
                         int                      initialDefaultValue,
                         int                      minRangeValue,
                         int                      maxRangeValue,
                         std::function<void(int)> onValueChange,
                         std::string              desc = "");
        void
            Render() override;

      private:
        int minValue;
        int maxValue;
    };
}
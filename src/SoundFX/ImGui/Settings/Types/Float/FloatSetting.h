#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class FloatSetting : public BaseSetting {
      public:
        FloatSetting(std::string                name,
                     float                      defaultValue,
                     std::function<void(float)> onChange,
                     std::string                desc = "");
        void
            Reset() override;
        [[nodiscard]] std::string
            GetName() const override;

      protected:
        std::string                name;
        float                      value;
        float                      defaultValue;
        std::function<void(float)> onChange;
    };
}
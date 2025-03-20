#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class FloatSetting : public BaseSetting {
      public:
        FloatSetting(std::string                name,
                     std::string                iniKey,
                     float                      defaultValue,
                     std::function<void(float)> onChange,
                     std::string                desc = "");
        void
            Reset() override;
        [[nodiscard]] std::string
            GetName() const override;

      protected:
        std::string                name;
        std::string                iniKey;
        float                      value;
        float                      defaultValue;
        std::function<void(float)> onChange;

        void
            Load();
        void
            Save() const;
    };
}
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

        [[nodiscard]] float
            GetValue() const {
            return value;
        }

        void
            SetValue(float newValue) {
            value = newValue;
            onChange(value);
        }

        [[nodiscard]] float
            GetDefaultValue() const {
            return defaultValue;
        }

        [[nodiscard]] const std::string &
            GetNameRef() const {
            return name;
        }

      protected:
        void
            Save() const;

      private:
        std::string                name;
        std::string                iniKey;
        float                      value;
        float                      defaultValue;
        std::function<void(float)> onChange;

        void
            Load();
    };
}
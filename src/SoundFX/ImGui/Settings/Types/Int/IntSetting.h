#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class IntSetting : public BaseSetting {
      public:
        IntSetting(std::string              name,
                   std::string              iniKey,
                   int                      defaultValue,
                   std::function<void(int)> onChange,
                   std::string              desc = "");
        void
            Reset() override;

        [[nodiscard]] std::string
            GetName() const override;

        [[nodiscard]] int
            GetValue() const {
            return value;
        }

        void
            SetValue(int newValue) {
            value = newValue;
            onChange(value);
        }

        [[nodiscard]] int
            GetDefaultValue() const {
            return defaultValue;
        }

      protected:
        void
            Save() const;

      private:
        std::string              name;
        std::string              iniKey;
        int                      value;
        int                      defaultValue;
        std::function<void(int)> onChange;

        void
            Load();
    };
}
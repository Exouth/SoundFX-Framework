#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class BoolSetting : public BaseSetting {
      public:
        BoolSetting(std::string               name,
                    std::string               iniKey,
                    bool                      defaultValue,
                    std::function<void(bool)> onChange,
                    std::string               desc = "");
        void
            Reset() override;

        [[nodiscard]] std::string
            GetName() const override;

        [[nodiscard]] bool
            GetValue() const {
            return value;
        }

        void
            SetValue(bool newValue) {
            value = newValue;
            onChange(value);
        }

        [[nodiscard]] bool
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
        std::string               name;
        std::string               iniKey;
        bool                      value;
        bool                      defaultValue;
        std::function<void(bool)> onChange;

        void
            Load();
    };
}
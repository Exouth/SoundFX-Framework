#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class ColorSetting final : public BaseSetting {
      public:
        ColorSetting(std::string                 settingName,
                     std::string                 iniKey,
                     const ImVec4               &defaultColor,
                     std::function<void(ImVec4)> onValueChange,
                     std::string                 desc = "");
        void
            Render() override;
        void
            Reset() override;

        [[nodiscard]] std::string
            GetName() const override;

        [[nodiscard]] ImVec4
            GetValue() const {
            return value;
        }

        void
            SetValue(const ImVec4 &newValue) {
            value = newValue;
            onChange(value);
        }

        [[nodiscard]] ImVec4
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
        std::string                 name;
        std::string                 iniKey;
        ImVec4                      value;
        ImVec4                      defaultValue;
        std::function<void(ImVec4)> onChange;

        void
            Load();
    };
}
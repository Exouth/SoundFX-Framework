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

      private:
        std::string                 name;
        std::string                 iniKey;
        ImVec4                      value;
        ImVec4                      defaultValue;
        std::function<void(ImVec4)> onChange;

        void
            Load();
        void
            Save() const;
    };
}
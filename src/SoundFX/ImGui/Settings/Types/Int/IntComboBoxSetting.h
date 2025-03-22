#pragma once

#include "ImGui/Settings/Types/Int/IntSetting.h"

namespace SoundFX {
    class IntComboBoxSetting final : public IntSetting {
      public:
        IntComboBoxSetting(std::string              settingName,
                           std::string              paramIniKey,
                           int                      initialDefaultValue,
                           std::vector<int>         options,
                           std::function<void(int)> onValueChange,
                           std::string              desc = "");
        void
            Render() override;

      private:
        std::vector<int> options;
    };
}
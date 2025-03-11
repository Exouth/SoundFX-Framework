#pragma once

#include "ImGui/Settings/Types/Bool/BoolSetting.h"

namespace SoundFX {
    class BoolCheckboxSetting final : public BoolSetting {
      public:
        BoolCheckboxSetting(std::string               settingName,
                            bool                      initialDefaultValue,
                            std::function<void(bool)> onValueChange,
                            std::string               desc = "");
        void
            Render() override;
    };
}
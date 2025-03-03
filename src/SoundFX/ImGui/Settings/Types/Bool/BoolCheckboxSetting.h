#pragma once

#include "ImGui/Settings/Types/Bool/BoolSetting.h"

namespace SoundFX {
    class BoolCheckboxSetting final : public BoolSetting {
      public:
        BoolCheckboxSetting(const std::string               &settingName,
                            bool                             initialDefaultValue,
                            const std::function<void(bool)> &onValueChange);
        void
            Render() override;
    };
}
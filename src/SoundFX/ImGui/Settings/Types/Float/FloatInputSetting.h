#pragma once

#include "ImGui/Settings/Types/Float/FloatSetting.h"

namespace SoundFX {
    class FloatInputSetting final : public FloatSetting {
      public:
        FloatInputSetting(std::string                settingName,
                          std::string                paramIniKey,
                          float                      initialDefaultValue,
                          std::function<void(float)> onValueChange,
                          std::string                desc = "");
        void
            Render() override;
    };
}
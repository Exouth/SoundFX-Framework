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

      protected:
        std::string               name;
        std::string               iniKey;
        bool                      value;
        bool                      defaultValue;
        std::function<void(bool)> onChange;

        void
            Load();
        void
            Save() const;
    };
}
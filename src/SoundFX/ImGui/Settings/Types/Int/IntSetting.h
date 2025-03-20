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

      protected:
        std::string              name;
        std::string              iniKey;
        int                      value;
        int                      defaultValue;
        std::function<void(int)> onChange;

        void
            Load();
        void
            Save() const;
    };
}
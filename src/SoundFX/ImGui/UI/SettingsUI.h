#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class SettingsUI {
      public:
        static void
            RenderSettingsUI();

      private:
        static std::vector<std::unique_ptr<BaseSetting>> settings;
        static void
            InitializeSettings();
    };
}
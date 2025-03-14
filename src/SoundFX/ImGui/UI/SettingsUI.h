#pragma once

#include "ImGui/Settings/Base/BaseSetting.h"

namespace SoundFX {
    class SettingsUI {
      public:
        static void
            RenderSettingsUI();

      private:
        static std::vector<std::unique_ptr<BaseSetting>> settings;
        static std::vector<std::unique_ptr<BaseSetting>> colorSettings;
        static void
            InitializeSettings();
    };
}
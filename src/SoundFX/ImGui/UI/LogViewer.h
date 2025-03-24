#pragma once
#include "imgui.h"
#include <deque>
#include <fstream>
#include <string>

constexpr auto ICON_FA_RELOAD = "\xef\x8b\xb1";  // Unicode: f2f1 / fa-rotate
constexpr auto ICON_FA_COPY   = "\xef\x83\x85";  // Unicode: f0c5 / fa-copy

namespace SoundFX {
    class LogViewer {
      public:
        static void
            Initialize();
        static void
            Shutdown();
        static void
            Render();
        static void
            ToggleVisibility();
        static void
            Show();
        static void
            Hide();
        static bool
            IsVisible();

      private:
        static bool                    visible;
        static bool                    autoUpdate;
        static std::deque<std::string> logLines;
        static size_t                  lastReadSize;
        static std::ifstream           logFile;

        static constexpr size_t MAX_LOG_LINES = 1000;

        static void
            ReadLogFile();
        static void
            ReloadLogFile();
        static void
            LoadSettings();
        static ImVec4
            GetLogLineColor(const std::string &line);
    };
}
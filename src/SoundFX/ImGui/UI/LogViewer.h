#pragma once
#include "imgui.h"
#include <deque>
#include <fstream>
#include <string>

constexpr auto ICON_FA_RELOAD = "\xef\x8b\xb1";  // Unicode: f2f1 / fa-rotate
constexpr auto ICON_FA_COPY   = "\xef\x83\x85";  // Unicode: f0c5 / fa-copy
constexpr auto ICON_FA_XMARK  = "\xef\x80\x8d";  // Unicode: f00d / fa-xmark
constexpr auto ICON_FA_ERASER = "\xef\x84\xad";  // Unicode: f12d / fa-eraser
constexpr auto ICON_FA_MGLASS = "\xef\x80\x82";  // Unicode: f002 / fa-magnifying-glass

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
        static std::string             filterType;
        static std::string             searchText;

        static constexpr auto LOG_FILE_NAME = "SoundFXFramework.log";

        static constexpr size_t MAX_LOG_LINES = 1000;

        static void
            ReadLogFile();
        static void
            ReloadLogFile();
        static void
            LoadSettings();
        static ImVec4
            GetLogLineColor(const std::string &line);
        static bool
            ShouldDisplayLine(const std::string &line);
        static bool
            MatchesSearch(const std::string &line);
        static void
            RenderLogTypeComboBox(
                const std::unordered_map<std::string, ImVec4> &logTypeToColorParam,
                std::string                                   &filterTypeParam);
        static void
            CopyLogsToClipboard();
        static void
            DisplayLogLines();
        static void
            RenderSearchBox();
        static void
            ClearLogs();
        static size_t
            CountFilteredLines();
        static void
            RenderCountLines();
    };
}
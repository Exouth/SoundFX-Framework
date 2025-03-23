#include "LogViewer.h"
#include "Config/ConfigManager.h"
#include "ImGui/Styles/Theme.h"
#include "Logger.h"

namespace SoundFX {

    bool                    LogViewer::visible    = true;
    bool                    LogViewer::autoUpdate = true;
    std::deque<std::string> LogViewer::logLines;
    size_t                  LogViewer::lastReadSize = 0;
    std::ifstream           LogViewer::logFile;

    void
        LogViewer::Initialize() {
        LoadSettings();

        if (const auto logDirectoryOpt = Logger::GetLogDirectory(); logDirectoryOpt.has_value()) {
            const auto logFilePath = logDirectoryOpt.value() / "SoundFXFramework.log";
            logFile.open(logFilePath, std::ios::in);
            if (logFile.is_open()) {
                lastReadSize = logFile.tellg();
            } else {
                logLines.emplace_back("Error: Could not open log file.");
                spdlog::error("Could not open log file.");
            }
        } else {
            logLines.emplace_back("Error: Log directory not found.");
            spdlog::error("Log directory not found.");
        }
    }

    void
        LogViewer::LoadSettings() {
        autoUpdate = Config::ConfigManager::GetInstance().GetValue<bool>(
            "UISettings", "LogViewerAutoUpdate", true);
        visible = Config::ConfigManager::GetInstance().GetValue<bool>(
            "UISettings", "LogViewerVisible", true);
    }

    void
        LogViewer::Shutdown() {
        if (logFile.is_open()) {
            logFile.close();
        }
        logLines.clear();
    }

    void
        LogViewer::ToggleVisibility() {
        visible ? Hide() : Show();
    }

    void
        LogViewer::Show() {
        visible = true;
        Config::ConfigManager::GetInstance().SetValue("UISettings", "LogViewerVisible", true);
        Config::ConfigManager::GetInstance().Save();
    }

    void
        LogViewer::Hide() {
        visible = false;
        Config::ConfigManager::GetInstance().SetValue("UISettings", "LogViewerVisible", false);
        Config::ConfigManager::GetInstance().Save();
    }

    bool
        LogViewer::IsVisible() {
        return visible;
    }

    void
        LogViewer::ReadLogFile() {
        if (!logFile.is_open()) {
            return;
        }

        logFile.seekg(static_cast<std::streamoff>(lastReadSize), std::ios::beg);

        std::string line;
        while (std::getline(logFile, line)) {
            if (logLines.size() >= MAX_LOG_LINES) {
                logLines.pop_front();
            }
            logLines.push_back(line);
        }

        lastReadSize = logFile.tellg();
    }

    void
        LogViewer::ReloadLogFile() {
        if (logFile.is_open()) {
            logFile.close();
        }

        if (const auto logDirectoryOpt = Logger::GetLogDirectory(); logDirectoryOpt.has_value()) {
            const auto logFilePath = logDirectoryOpt.value() / "SoundFXFramework.log";

            if (const size_t currentSize = file_size(logFilePath); currentSize == lastReadSize) {
                return;
            }

            logFile.open(logFilePath, std::ios::in);
            if (logFile.is_open()) {
                logLines.clear();
                lastReadSize = 0;
                ReadLogFile();
            } else {
                logLines.emplace_back("Error: Could not open log file during reload.");
                spdlog::error("Could not open log file during reload.");
            }
        } else {
            logLines.emplace_back("Error: Log directory not found during reload.");
            spdlog::error("Log directory not found during reload.");
        }
    }

    void
        LogViewer::Render() {
        if (!IsVisible()) {
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
        bool open = true;
        if (!ImGui::Begin("Log Viewer", &open)) {
            ImGui::End();
            return;
        }

        if (!open) {
            ToggleVisibility();
            ImGui::End();
            return;
        }

        if (autoUpdate) {
            ReloadLogFile();
        }

        ImGui::SameLine();
        if (ImGui::Button((std::string(ICON_FA_RELOAD) + " Reload Log").c_str())) {
            ReloadLogFile();
        }

        ImGui::SameLine();
        if (ImGui::Button((std::string(ICON_FA_COPY) + " Copy All").c_str())) {
            std::string allLogText;
            for (const auto &line : logLines) {
                allLogText += line + "\n";
            }
            ImGui::SetClipboardText(allLogText.c_str());
        }

        ImGui::SameLine();
        if (ImGui::Checkbox("Auto-Update", &autoUpdate)) {
            Config::ConfigManager::GetInstance().SetValue(
                "UISettings", "LogViewerAutoUpdate", autoUpdate);
            Config::ConfigManager::GetInstance().Save();
        }

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(
                "Keeps the log up-to-date during the game.\n"
                "When enabled, the log viewer continuously shows the latest entries.\n"
                "If disabled, use 'Reload Log' to manually refresh the log.");
            ImGui::EndTooltip();
        }

        ImGui::Separator();

        ImGui::BeginChild(
            "LogScroll", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        if (!logLines.empty()) {
            const size_t startIdx =
                logLines.size() > MAX_LOG_LINES ? logLines.size() - MAX_LOG_LINES : 0;

            for (size_t i = startIdx; i < logLines.size(); ++i) {
                const std::string &line = logLines[i];
                if (!line.empty()) {
                    ImGui::TextUnformatted(line.c_str());
                }
            }

            if (autoUpdate) {
                ImGui::SetScrollHereY(1.0f);
            }

        } else {
            ImGui::Text("No log entries available.");
        }

        ImGui::EndChild();

        ImGui::Spacing();
        Theme::RenderFooterLine();
        ImGui::End();
    }

}
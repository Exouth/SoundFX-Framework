#include "LogViewer.h"
#include "Config/ConfigManager.h"
#include "ImGui/Styles/Theme.h"
#include "Logger.h"
#include <algorithm>

namespace SoundFX {

    bool                    LogViewer::visible    = true;
    bool                    LogViewer::autoUpdate = true;
    std::deque<std::string> LogViewer::logLines;
    size_t                  LogViewer::lastReadSize = 0;
    std::ifstream           LogViewer::logFile;
    std::string             LogViewer::filterType = "All";
    std::string             LogViewer::searchText;

    const std::unordered_map<std::string, ImVec4> logTypeToColor = {
        {"[All]", ImVec4(0.9f, 0.9f, 0.9f, 1.0f)},        // All (White in Combobox)
        {"[critical]", ImVec4(0.75f, 0.1f, 0.1f, 1.0f)},  // Red
        {"[error]", ImVec4(1.0f, 0.2f, 0.2f, 1.0f)},      // Light Red
        {"[warning]", ImVec4(1.0f, 0.65f, 0.0f, 1.0f)},   // Orange
        {"[info]", ImVec4(0.25f, 0.58f, 0.96f, 1.0f)},    // Blue
        {"other", ImVec4(0.5f, 0.5f, 0.5f, 1.0f)}         // Grey
    };

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
        filterType = Config::ConfigManager::GetInstance().GetValue<std::string>(
            "UISettings", "LogViewerFilterType", "All");
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
        if (!logFile.is_open()) {
            const std::string errorMessage = "Error: Log file is not open during reload.";
            logLines.emplace_back(errorMessage);
            spdlog::error(errorMessage);
            return;
        }

        const auto logDirectoryOpt = Logger::GetLogDirectory();
        if (!logDirectoryOpt.has_value()) {
            const std::string errorMessage = "Error: Log directory not found during reload.";
            logLines.emplace_back(errorMessage);
            spdlog::error(errorMessage);
            return;
        }

        const auto logFilePath = logDirectoryOpt.value() / "SoundFXFramework.log";

        if (!is_regular_file(logFilePath)) {
            const std::string errorMessage =
                "Error: Log file does not exist or is not a regular file.";
            logLines.emplace_back(errorMessage);
            spdlog::error(errorMessage);
            return;
        }

        const size_t currentSize = file_size(logFilePath);
        if (currentSize == lastReadSize) {
            return;
        }

        logFile.close();
        logFile.open(logFilePath, std::ios::in);
        if (!logFile.is_open()) {
            const std::string errorMessage = "Error: Could not open log file during reload.";
            logLines.emplace_back(errorMessage);
            spdlog::error(errorMessage);
            return;
        }

        logLines.clear();
        lastReadSize = 0;
        ReadLogFile();
    }

    ImVec4
        LogViewer::GetLogLineColor(const std::string &line) {
        const auto it = std::ranges::find_if(logTypeToColor, [&line](const auto &pair) {
            return line.find(pair.first) != std::string::npos;
        });

        return it != logTypeToColor.end() ? it->second : logTypeToColor.at("other");
    }

    bool
        LogViewer::ShouldDisplayLine(const std::string &line) {
        if (filterType == "All") {
            return true;
        }

        if (line.find("[" + filterType + "]") != std::string::npos) {
            return true;
        }

        if (filterType == "other" && line.find('[') == std::string::npos) {
            return true;
        }

        return false;
    }

    void
        LogViewer::RenderLogTypeComboBox(
            const std::unordered_map<std::string, ImVec4> &logTypeToColorParam,
            std::string                                   &filterTypeParam) {

        if (ImGui::BeginCombo("Filter by Type", filterTypeParam.c_str())) {
            for (const auto &[fst, snd] : logTypeToColorParam) {
                const std::string &formattedType = fst;

                std::string displayType = formattedType;
                if (formattedType.front() == '[' && formattedType.back() == ']') {
                    displayType = formattedType.substr(1, formattedType.size() - 2);
                }

                const bool isSelected = filterTypeParam == displayType;
                ImVec4     color      = snd;

                ImGui::PushStyleColor(ImGuiCol_Text, color);
                if (ImGui::Selectable(displayType.c_str(), isSelected)) {
                    filterTypeParam = displayType;
                    Config::ConfigManager::GetInstance().SetValue(
                        "UISettings", "LogViewerFilterType", filterTypeParam);
                    Config::ConfigManager::GetInstance().Save();
                }
                ImGui::PopStyleColor();

                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
    }

    void
        LogViewer::CopyLogsToClipboard() {
        std::string filteredLogText;
        for (const auto &line : logLines) {
            if (!line.empty() && ShouldDisplayLine(line) && MatchesSearch(line)) {
                filteredLogText += line + "\n";
            }
        }
        ImGui::SetClipboardText(filteredLogText.c_str());
    }

    void
        LogViewer::DisplayLogLines() {
        const size_t startIdx =
            logLines.size() > MAX_LOG_LINES ? logLines.size() - MAX_LOG_LINES : 0;

        static size_t previousLogSize = 0;
        const bool    newLogsAdded    = logLines.size() > previousLogSize;

        for (size_t i = startIdx; i < logLines.size(); ++i) {
            const std::string &line = logLines[i];
            if (!line.empty() && ShouldDisplayLine(line) && MatchesSearch(line)) {
                ImVec4 color = GetLogLineColor(line);
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::TextUnformatted(line.c_str());
                ImGui::PopStyleColor();
            }
        }

        if (autoUpdate && newLogsAdded) {
            ImGui::SetScrollHereY(1.0f);
        }

        previousLogSize = logLines.size();
    }

    bool
        LogViewer::MatchesSearch(const std::string &line) {
        if (searchText.empty()) {
            return true;
        }
        return line.find(searchText) != std::string::npos;
    }

    void
        LogViewer::RenderSearchBox() {
        char searchBuffer[256];
        strncpy_s(searchBuffer, sizeof(searchBuffer), searchText.c_str(), _TRUNCATE);

        if (ImGui::InputTextWithHint(
                "##SearchBox", "Search...", searchBuffer, sizeof(searchBuffer))) {
            searchText = searchBuffer;
        }

        if (!searchText.empty()) {
            ImGui::SameLine();
            if (ImGui::Button(ICON_FA_XMARK)) {
                searchText.clear();
            }
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
            CopyLogsToClipboard();
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(150);
        RenderLogTypeComboBox(logTypeToColor, filterType);

        if (ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(
                "Select a log type to filter the displayed log entries.\n"
                "Options: All, info, warning, error, critical, other.");
            ImGui::Spacing();
            ImGui::TextUnformatted(
                "(other: Displays log entries that do not contain a specific log type prefix.)");
            ImGui::EndTooltip();
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

        ImGui::SetNextItemWidth(400);
        RenderSearchBox();

        ImGui::BeginChild(
            "LogScroll", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

        if (!logLines.empty()) {
            DisplayLogLines();
        } else {
            ImGui::Text("No log entries available.");
        }

        ImGui::EndChild();

        ImGui::Spacing();
        Theme::RenderFooterLine();
        ImGui::End();
    }

}
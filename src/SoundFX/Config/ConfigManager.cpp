#include "ConfigManager.h"

namespace SoundFX::Config {

    ConfigManager &
        ConfigManager::GetInstance() {
        static ConfigManager instance;
        return instance;
    }

    bool
        ConfigManager::Load() {
        std::lock_guard lock(iniMutex_);
        ini_.SetUnicode();
        if (const SI_Error rc = ini_.LoadFile(iniFilePath_); rc < 0) {
            const auto errorMsg = fmt::format(
                "INI file '{}' not found! The plugin can't start without this file. Please "
                "reinstall or restore the missing INI file.",
                iniFilePath_);

            spdlog::error(errorMsg);

            MessageBoxA(
                nullptr, errorMsg.c_str(), "SoundFX Framework - Fatal Error", MB_OK | MB_ICONERROR);

            std::terminate();
        }

        spdlog::info("INI file loaded successfully: {}", iniFilePath_);
        return true;
    }

    void
        ConfigManager::Save() {
        std::lock_guard lock(iniMutex_);
        if (const SI_Error rc = ini_.SaveFile(iniFilePath_); rc < 0) {
            spdlog::error("Failed to save INI file: {}", iniFilePath_);
        }
    }

}
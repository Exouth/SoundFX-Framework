#pragma once

#include "Utility.h"
#include <SimpleIni.h>
#include <imgui.h>

namespace SoundFX::Config {

    class ConfigManager {
      public:
        static ConfigManager &
            GetInstance();

        bool
            Load();
        void
            Save();

        ConfigManager(const ConfigManager &) = delete;
        ConfigManager &
            operator=(const ConfigManager &) = delete;

        ConfigManager(ConfigManager &&) noexcept = delete;
        ConfigManager &
            operator=(ConfigManager &&) noexcept = delete;

        template <typename T>
        T
            GetValue(const char *section, const char *key, const T &defaultValue = T()) const;

        template <typename T>
        void
            SetValue(const char *section, const char *key, const T &value);

      private:
        ConfigManager()  = default;
        ~ConfigManager() = default;

        mutable std::mutex iniMutex_;
        CSimpleIniA        ini_;

        inline static auto iniFilePath_ = "Data/SKSE/Plugins/SoundFXFramework.ini";
    };

    template <>
    inline int
        ConfigManager::GetValue<int>(const char *section,
                                     const char *key,
                                     const int  &defaultValue) const {
        std::lock_guard lock(iniMutex_);
        return static_cast<int>(ini_.GetLongValue(section, key, defaultValue));
    }

    template <>
    inline float
        ConfigManager::GetValue<float>(const char  *section,
                                       const char  *key,
                                       const float &defaultValue) const {
        std::lock_guard lock(iniMutex_);
        return static_cast<float>(ini_.GetDoubleValue(section, key, defaultValue));
    }

    template <>
    inline bool
        ConfigManager::GetValue<bool>(const char *section,
                                      const char *key,
                                      const bool &defaultValue) const {
        std::lock_guard lock(iniMutex_);
        return ini_.GetBoolValue(section, key, defaultValue);
    }

    template <>
    inline std::string
        ConfigManager::GetValue<std::string>(const char        *section,
                                             const char        *key,
                                             const std::string &defaultValue) const {
        std::lock_guard lock(iniMutex_);
        return ini_.GetValue(section, key, defaultValue.c_str());
    }

    template <>
    inline void
        ConfigManager::SetValue<int>(const char *section, const char *key, const int &value) {
        std::lock_guard lock(iniMutex_);
        ini_.SetLongValue(section, key, value);
    }

    template <>
    inline void
        ConfigManager::SetValue<float>(const char *section, const char *key, const float &value) {
        std::lock_guard lock(iniMutex_);
        ini_.SetDoubleValue(section, key, value);
    }

    template <>
    inline void
        ConfigManager::SetValue<bool>(const char *section, const char *key, const bool &value) {
        std::lock_guard lock(iniMutex_);
        ini_.SetBoolValue(section, key, value);
    }

    template <>
    inline void
        ConfigManager::SetValue<std::string>(const char        *section,
                                             const char        *key,
                                             const std::string &value) {
        std::lock_guard lock(iniMutex_);
        ini_.SetValue(section, key, value.c_str());
    }

    template <>
    inline ImVec4
        ConfigManager::GetValue<ImVec4>(const char   *section,
                                        const char   *key,
                                        const ImVec4 &defaultValue) const {
        std::lock_guard lock(iniMutex_);
        std::string     value = ini_.GetValue(section, key, "");

        return value.empty() ? defaultValue : HexToImVec4(value);
    }

    template <>
    inline void
        ConfigManager::SetValue<ImVec4>(const char *section, const char *key, const ImVec4 &value) {
        std::lock_guard lock(iniMutex_);

        ini_.SetValue(section, key, ImVec4ToHex(value).c_str());
    }

    bool
        Load();
    void
        Save();

}
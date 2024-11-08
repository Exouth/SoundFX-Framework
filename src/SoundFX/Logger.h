#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <spdlog/logger.h>

namespace SoundFX {
    class Logger {
      public:
        static void
            Initialize();
        static spdlog::logger &
            Get();
        static std::optional<std::filesystem::path>
            GetLogDirectory();
    };
}
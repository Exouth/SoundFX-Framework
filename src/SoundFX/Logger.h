#pragma once

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
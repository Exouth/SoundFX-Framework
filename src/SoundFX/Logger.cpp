#include "Logger.h"
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#ifdef NDEBUG
// File_Sink
#else
#    include <spdlog/sinks/msvc_sink.h>
#endif

void
    SoundFX::Logger::Initialize() {
    auto path = GetLogDirectory();
    if (!path) {
        throw std::runtime_error("Log directory could not be found.");
    }
    *path /= "SoundFXFramework.log";

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

#ifdef NDEBUG
    auto logger = std::make_shared<spdlog::logger>("global", std::move(file_sink));
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::warn);
#else
    auto msvc_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
    auto logger =
        std::make_shared<spdlog::logger>("global", spdlog::sinks_init_list {msvc_sink, file_sink});
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
#endif

    logger->flush_on(spdlog::level::info);
    spdlog::set_default_logger(std::move(logger));
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] %v");
    spdlog::info("Logger initialized");
}

spdlog::logger &
    SoundFX::Logger::Get() {
    return *spdlog::default_logger();
}

std::optional<std::filesystem::path>
    SoundFX::Logger::GetLogDirectory() {
    auto path = SKSE::log::log_directory();
    if (!path) {
        throw std::runtime_error("Log directory could not be found.");
    }
    return path;
}
#include "SoundFX/Logger.h"
#include "SoundFX/JSONLoader.h"

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    Init(skse);

    try {
        SoundFX::Logger::Initialize();
    } catch (const std::exception &e) {
        SKSE::stl::report_and_fail(fmt::format("Failed to initialize logger: {}", e.what()));
    }

    spdlog::info("Plugin loaded successfully");

    return true;
}
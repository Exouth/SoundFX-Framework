#include "SoundFX/EventHandlerManager.h"
#include "SoundFX/JSONLoader.h"
#include "SoundFX/Logger.h"

void
    OnSKSEMessage(SKSE::MessagingInterface::Message *msg) {
    switch (msg->type) {
    case SKSE::MessagingInterface::kNewGame:
    case SKSE::MessagingInterface::kPostLoadGame:
        {
            auto                               &jsonLoader = SoundFX::JSONLoader::GetInstance();
            static SoundFX::EventHandlerManager eventManager(jsonLoader);
            eventManager.InitializeEventHandlers();
            break;
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    Init(skse);

    SoundFX::Logger::Initialize();

    SoundFX::JSONLoader::GetInstance().load();

    SKSE::GetMessagingInterface()->RegisterListener(OnSKSEMessage);
    spdlog::info("Plugin loaded successfully");

    return true;
}
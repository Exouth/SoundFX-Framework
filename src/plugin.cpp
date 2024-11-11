#include "SoundFX/EventHandlerManager.h"
#include "SoundFX/JSONLoader.h"
#include "SoundFX/Logger.h"

void
    OnSKSEMessage(SKSE::MessagingInterface::Message *msg) {
    if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
        {
            auto                               &jsonLoader = SoundFX::JSONLoader::GetInstance();
            static SoundFX::EventHandlerManager eventManager(jsonLoader);
            eventManager.InitializeEventHandlers();

            // For autoload Testing Save (Creating AutoExec File for SKSE)
            auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
            if (scriptFactory) {
                auto loadCommand = scriptFactory->Create();
                if (loadCommand) {
                    loadCommand->SetCommand(
                        "load "
                        "Save3_6C8DC6E8_0_507269736F6E65726466676664_Tamriel_000000_20241111115503_"
                        "1_1");
                    loadCommand->CompileAndRun(nullptr, RE::COMPILER_NAME::kSystemWindowCompiler);
                    spdlog::info("Load command executed.");
                }
            }
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
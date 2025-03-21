/*
 * This file is part of SoundFX-Framework.
 *
 * Copyright (C) 2024-2025 Exouth.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "Config/ConfigManager.h"
#include "Events/EventHandlerManager.h"
#include "Hooks/HookManager.h"
#include "JSONLoader.h"
#include "Logger.h"
#include "Sound/SoundManager.h"

void
    OnSKSEMessage(SKSE::MessagingInterface::Message *msg) {
    if (msg->type == SKSE::MessagingInterface::kDataLoaded) {
        {
            auto                               &jsonLoader = SoundFX::JSONLoader::GetInstance();
            static SoundFX::EventHandlerManager eventManager(&jsonLoader);
            eventManager.InitializeEventHandlers();

            SoundFX::HookManager::GetInstance().InstallHooks();

            SoundFX::SoundManager::GetInstance().Initialize();

            // For autoload Testing Save (Creating AutoExec File for SKSE)
            auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
            if (scriptFactory) {
                auto loadCommand = scriptFactory->Create();
                if (loadCommand) {
                    loadCommand->SetCommand(
                        "load "
                        "Save3_83E44258_0_507269736F6E6572647366736466736466_Tamriel_000001_"
                        "20250226114851_1_1");
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

    SoundFX::Config::ConfigManager::GetInstance().Load();

    SKSE::AllocTrampoline(64);  // For 2 Hooks

    SoundFX::JSONLoader::GetInstance().load();

    SKSE::GetMessagingInterface()->RegisterListener(OnSKSEMessage);
    spdlog::info("Plugin loaded successfully");

    return true;
}
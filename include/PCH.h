#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _USE_MATH_DEFINES

#include <RE/Skyrim.h>
#include <RE/B/BSTEvent.h>
#include <RE/B/BSSoundHandle.h>
#include <RE/N/NiPoint3.h>
#include <RE/T/TESCombatEvent.h>
#include <RE/T/TESContainerChangedEvent.h>
#include <RE/T/TESEquipEvent.h>
#include <RE/T/TESHitEvent.h>
#include <RE/T/TESQuestStageEvent.h>
#include <RE/T/TESSpellCastEvent.h>
#include <RE/T/TESCombatEvent.h>
#include <RE/S/ScriptEventSourceHolder.h>
#include "SKSE/SKSE.h"
#include "REL/Relocation.h"

#include <Windows.h>
#undef GetObject
#include <dinput.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <nlohmann/json.hpp>

#include <SimpleIni.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <optional>
#include <filesystem>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <future>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <thread>
#include <numbers>

#include "Logger.h"
#include "JSONLoader.h"
#include "Events/EventHandlerManager.h"
#include "Sound/CustomSoundDescriptor.h"
#include "Sound/SoundUtil.h"
#include "Utility.h"
#include "TaskScheduler.h"

using namespace std::literals;
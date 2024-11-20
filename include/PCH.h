#pragma once

#include <RE/Skyrim.h>
#include <RE/B/BSTEvent.h>
#include <RE/B/BSSoundHandle.h>
#include "SKSE/SKSE.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <nlohmann/json.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

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

using namespace std::literals;

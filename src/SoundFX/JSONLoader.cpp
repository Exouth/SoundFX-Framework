#include "JSONLoader.h"

namespace SoundFX {

    JSONLoader::JSONLoader(const std::string &filePath) : filePath(filePath) {
    }

    bool
        JSONLoader::load() {
        if (!isLoaded) {
            isLoaded = loadJSON();
        }
        return isLoaded;
    }

    bool
        JSONLoader::loadJSON() {
        spdlog::info("Loading JSON file from path: {}", filePath);

        std::ifstream file(filePath);
        if (!file) {
            spdlog::error("Could not open JSON file: {}", filePath);
            return false;
        }

        try {
            file >> jsonData;
            spdlog::info("JSON file loaded successfully.");

            if (!jsonData.contains("events")) {
                spdlog::error("No 'events' key found in JSON.");
                return false;
            }

            for (const auto &category : jsonData["events"].items()) {
                parseCategory(category.key());
            }

            return true;
        } catch (const std::exception &e) {
            spdlog::error("Failed to parse JSON file: {}", e.what());
            return false;
        }
    }

    void
        JSONLoader::parseCategory(const std::string &category) {
        spdlog::debug("Parsing category: {}", category);

        if (!jsonData["events"].contains(category)) {
            spdlog::warn("Category '{}' not found in events.", category);
            return;
        }

        for (const auto &[itemName, itemData] : jsonData["events"][category].items()) {
            ItemEvents item;
            item.name = itemName;

            if (itemData.contains("editorID")) {
                item.editorID = itemData["editorID"].get<std::string>();
            } else {
                spdlog::warn("No editorID for item '{}' in category '{}'. Skipping item.",
                             itemName,
                             category);
                continue;
            }

            if (itemData.contains("pluginName")) {
                item.pluginName = itemData["pluginName"].get<std::string>();
            } else {
                spdlog::warn("No pluginName for item '{}' in category '{}'. Skipping item.",
                             itemName,
                             category);
                continue;
            }

            if (!itemData.contains("events")) {
                spdlog::warn("No 'events' array for item '{}' in category '{}'. Skipping item.",
                             itemName,
                             category);
                continue;
            }

            for (const auto &jsonEvent : itemData["events"]) {
                if (auto parsed = parseEvent(jsonEvent, itemName); parsed.has_value()) {
                    item.events.push_back(*parsed);
                }
            }

            allItems[category][itemName] = std::move(item);
            spdlog::debug("Item '{}' parsed successfully in category '{}'.", itemName, category);
        }
    }

    std::optional<Event>
        JSONLoader::parseEvent(const nlohmann::json &jsonEvent, const std::string &itemName) {
        Event event {
            .chance = 1.0f, .repeatFrequency = 1, .volume = -1.0f, .isAbsoluteVolume = true};

        try {
            if (jsonEvent.contains("type")) {
                event.type = jsonEvent["type"].get<std::string>();
            } else {
                spdlog::warn("Event missing 'type' key. Skipping event.");
                return std::nullopt;
            }

            if (jsonEvent.contains("soundEffect")) {
                event.soundEffect = jsonEvent["soundEffect"].get<std::string>();
            } else {
                spdlog::warn("Event missing 'soundEffect' key. Skipping event.");
                return std::nullopt;
            }

            event.chance          = jsonEvent.value("chance", 0.0f);
            event.repeatFrequency = jsonEvent.value("repeatFrequency", 1);

            const bool hasAbsolute = jsonEvent.contains("volumeAbsolute");
            const bool hasRelative = jsonEvent.contains("volumeRelativeToMaster");

            if (hasAbsolute && hasRelative) {
                spdlog::error(
                    "Both 'volumeAbsolute' and 'volumeRelativeToMaster' are set for event '{}' in "
                    "item '{}'. Skipping Event Type.",
                    event.type,
                    itemName);
                return std::nullopt;
            }

            if (hasAbsolute) {
                event.volume           = jsonEvent["volumeAbsolute"].get<float>();
                event.isAbsoluteVolume = true;
            } else if (hasRelative) {
                event.volume           = jsonEvent["volumeRelativeToMaster"].get<float>() / 100.0f;
                event.isAbsoluteVolume = false;
            }

            if (jsonEvent.contains("details")) {
                const auto &details = jsonEvent["details"];
                if (details.contains("attackType")) {
                    event.details.attackType = details["attackType"].get<std::string>();
                }
                if (details.contains("hitType")) {
                    event.details.hitType = details["hitType"].get<std::string>();
                }
                if (details.contains("environment")) {
                    event.details.environment = details["environment"].get<std::string>();
                }
                if (details.contains("onlyAt")) {
                    event.details.onlyAt = details["onlyAt"].get<std::string>();
                }
            }
        } catch (const std::exception &e) {
            spdlog::error("Failed to parse event: {}", e.what());
            return std::nullopt;
        }

        return event;
    }

    const std::unordered_map<std::string, ItemEvents> &
        JSONLoader::getItems(const std::string &category) const {
        spdlog::debug("Accessing category '{}'", category);

        static const std::unordered_map<std::string, ItemEvents> empty;
        if (allItems.contains(category)) {
            spdlog::debug("Category '{}' found, returning items.", category);
            return allItems.at(category);
        }

        return empty;
    }

}
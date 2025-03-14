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
                item.events.push_back(parseEvent(jsonEvent));
            }

            allItems[category][itemName] = std::move(item);
            spdlog::debug("Item '{}' parsed successfully in category '{}'.", itemName, category);
        }
    }

    Event
        JSONLoader::parseEvent(const nlohmann::json &jsonEvent) {
        Event event {.chance = 1.0f, .repeatFrequency = 1, .volume = 1.0f};

        try {
            if (jsonEvent.contains("type")) {
                event.type = jsonEvent["type"].get<std::string>();
            } else {
                spdlog::warn("Event missing 'type' key. Skipping event.");
                return event;
            }

            if (jsonEvent.contains("soundEffect")) {
                event.soundEffect = jsonEvent["soundEffect"].get<std::string>();
            } else {
                spdlog::warn("Event missing 'soundEffect' key. Skipping event.");
                return event;
            }

            event.chance          = jsonEvent.value("chance", 0.0f);
            event.repeatFrequency = jsonEvent.value("repeatFrequency", 1);
            event.volume          = jsonEvent.value("volume", 1.0f);

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
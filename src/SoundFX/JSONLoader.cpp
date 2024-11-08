#include "JSONLoader.h"
#include <fstream>
#include <spdlog/spdlog.h>

namespace SoundFX {

    JSONLoader::JSONLoader(const std::string &filePath) : filePath(filePath) {
    }

    bool
        JSONLoader::load() {
        std::ifstream file(filePath);
        if (!file) {
            spdlog::error("Could not open JSON file: {}", filePath);
            return false;
        }
        file >> jsonData;

        for (const auto &category : jsonData["events"].items()) {
            parseCategory(category.key());
        }

        return true;
    }

    void
        JSONLoader::parseCategory(const std::string &category) {
        for (const auto &[itemName, itemData] : jsonData["events"][category].items()) {
            ItemEvents item;
            item.formID = itemData["formID"].get<std::string>();

            for (const auto &jsonEvent : itemData["events"]) {
                item.events.push_back(parseEvent(jsonEvent));
            }

            allItems[category][itemName] = std::move(item);
        }
    }

    Event
        JSONLoader::parseEvent(const nlohmann::json &jsonEvent) {
        Event event;
        event.type            = jsonEvent["type"].get<std::string>();
        event.soundEffect     = jsonEvent["soundEffect"].get<std::string>();
        event.chance          = jsonEvent["chance"].get<float>();
        event.repeatFrequency = jsonEvent["repeatFrequency"].get<int>();
        event.volume          = jsonEvent["volume"].get<float>();

        if (jsonEvent.contains("details")) {
            const auto &details = jsonEvent["details"];
            if (details.contains("attackType"))
                event.details.attackType = details["attackType"].get<std::string>();
            if (details.contains("hitType"))
                event.details.hitType = details["hitType"].get<std::string>();
            if (details.contains("environment"))
                event.details.environment = details["environment"].get<std::string>();
            if (details.contains("enemyCount"))
                event.details.enemyCount = details["enemyCount"].get<int>();
        }

        return event;
    }

    const std::unordered_map<std::string, ItemEvents> &
        JSONLoader::getItems(const std::string &category) const {
        static const std::unordered_map<std::string, ItemEvents> empty;
        if (allItems.contains(category)) {
            return allItems.at(category);
        }
        return empty;
    }

}
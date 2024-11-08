#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace SoundFX {

    struct EventDetails {
        std::optional<std::string> attackType;
        std::optional<std::string> hitType;
        std::optional<std::string> environment;
        std::optional<int>         enemyCount;
    };

    struct Event {
        std::string  type;
        std::string  soundEffect;
        float        chance;
        int          repeatFrequency;
        float        volume;
        EventDetails details;
    };

    struct ItemEvents {
        std::string        formID;
        std::vector<Event> events;
    };

    class JSONLoader {
      public:
        JSONLoader(const std::string &filePath);
        bool
            load();
        const std::unordered_map<std::string, ItemEvents> &
            getItems(const std::string &category) const;

      private:
        std::string                                                                  filePath;
        nlohmann::json                                                               jsonData;
        std::unordered_map<std::string, std::unordered_map<std::string, ItemEvents>> allItems;

        void
            parseCategory(const std::string &category);
        Event
            parseEvent(const nlohmann::json &jsonEvent);
    };

}
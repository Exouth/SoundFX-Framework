#pragma once

namespace SoundFX {

    struct EventDetails {
        std::optional<std::string> attackType;
        std::optional<std::string> hitType;
        std::optional<std::string> environment;
        std::optional<std::string> onlyAt;
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
        std::string        name;
        std::string        editorID;
        std::string        pluginName;
        std::vector<Event> events;
    };

    class JSONLoader {
      public:
        static JSONLoader &
            GetInstance() {
            static JSONLoader instance("Data/SKSE/Plugins/SoundFXFramework.json");
            return instance;
        }
        bool
            load();
        const std::unordered_map<std::string, ItemEvents> &
            getItems(const std::string &category) const;

      private:
        explicit JSONLoader(const std::string &filePath);
        bool
                                                                                     loadJSON();
        std::string                                                                  filePath;
        nlohmann::json                                                               jsonData;
        std::unordered_map<std::string, std::unordered_map<std::string, ItemEvents>> allItems;
        bool isLoaded = false;

        void
            parseCategory(const std::string &category);
        static Event
            parseEvent(const nlohmann::json &jsonEvent);

        JSONLoader(const JSONLoader &) = delete;
        JSONLoader &
            operator=(const JSONLoader &) = delete;
    };

}
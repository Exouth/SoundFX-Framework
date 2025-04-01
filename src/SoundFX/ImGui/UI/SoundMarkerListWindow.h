#pragma once

#include "Sound/SoundManager.h"

constexpr auto ICON_FA_ELLIPSIS = "\xef\x85\x81";  // Unicode: f141 / fa-ellipsis

namespace SoundFX {
    class SoundMarkerListWindow {
      public:
        static void
            Render();

      private:
        static void
            RenderActiveSoundCount(size_t count);

        static void
            RenderSortOptions(int &sortMode);

        static void
            SortActiveSounds(std::vector<std::shared_ptr<SoundManager::ActiveSound>> &activeSounds,
                             int                                                      sortMode);

        enum class SoundAction : std::uint8_t {
            None,
            Stop,
            Reload,
        };

        static void
            RenderSoundTable(
                const std::vector<std::shared_ptr<SoundManager::ActiveSound>> &activeSounds,
                const RE::PlayerCharacter                                     *player,
                std::optional<std::pair<std::size_t, SoundAction>>            &selectedAction);
    };
}
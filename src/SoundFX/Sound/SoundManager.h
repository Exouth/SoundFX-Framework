#pragma once

#include "CustomSoundDescriptor.h"

namespace SoundFX {

    class SoundManager {
      public:
        SoundManager(const SoundManager &) = delete;
        SoundManager &
            operator=(const SoundManager &) = delete;
        SoundManager(SoundManager &&)       = delete;
        SoundManager &
            operator=(SoundManager &&) = delete;

        struct ActiveSound {
            std::size_t                                               id;
            std::string                                               name;
            std::string                                               eventType;
            std::string                                               soundEffect;
            std::variant<RE::NiPoint3, std::function<RE::NiPoint3()>> position;
            float                                                     referenceDistance;
            float                                                     maxDistance;
            bool                                                      is3D;
            std::shared_ptr<CustomSoundDescriptor>                    descriptor;
            ALuint                                                    sourceID = 0;
            std::chrono::steady_clock::time_point                     startTime;
            // Prevents sounds from being removed too early due to initialization delays.

            [[nodiscard]] bool
                IsPlaying() const;

            [[nodiscard]] RE::NiPoint3
                GetPosition() const;
        };

        static SoundManager &
            GetInstance();

        void
            Initialize();

        static void
            PlaySound(const std::string  &name,
                      const std::string  &eventType,
                      const std::string  &soundEffect,
                      const RE::NiPoint3 &position,
                      float               referenceDistance,
                      float               maxDistance,
                      bool                is3D);
        static bool
            IsSoundPlaying(std::size_t soundID);
        static void
            Update();
        static const std::vector<ActiveSound> &
            GetActiveSounds();
        static std::vector<ActiveSound>
            GetSortedActiveSounds();
        static bool
            Is3DSoundEnabled();

      private:
        SoundManager()  = default;
        ~SoundManager() = default;

        static std::size_t
            GenerateSoundID(const std::string  &name,
                            const std::string  &eventType,
                            const std::string  &soundEffect,
                            const RE::NiPoint3 &position);

        static std::vector<ActiveSound> activeSounds;
        static std::atomic<bool>        is3DSoundEnabled;
        static TaskScheduler            scheduler;
    };
}
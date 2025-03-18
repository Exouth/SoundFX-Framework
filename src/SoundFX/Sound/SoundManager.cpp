#include "SoundManager.h"
#include "Sound3DUtil.h"

namespace SoundFX {

    std::vector<SoundManager::ActiveSound> SoundManager::activeSounds;
    std::atomic<bool>                      SoundManager::is3DSoundEnabled = true;
    TaskScheduler                          SoundManager::scheduler;

    SoundManager &
        SoundManager::GetInstance() {
        static SoundManager instance;
        return instance;
    }

    void
        SoundManager::Initialize() {
        if (!Sound3DUtil::InitializeOpenAL()) {
            is3DSoundEnabled = false;
            spdlog::error("3D sound has been deactivated due to an error.");

            return;
        }

        scheduler.Start(500);
        scheduler.AddTask([] { Update(); }, true);
    }

    std::size_t
        SoundManager::GenerateSoundID(const std::string  &name,
                                      const std::string  &eventType,
                                      const std::string  &soundEffect,
                                      const RE::NiPoint3 &position) {
        constexpr std::hash<std::string> strHash;
        constexpr std::hash<float>       floatHash;
        const auto now = std::chrono::system_clock::now().time_since_epoch().count();

        return strHash(name) ^ strHash(eventType) ^ strHash(soundEffect) ^ floatHash(position.x)
             ^ floatHash(position.y) ^ floatHash(position.z) ^ now;
    }

    void
        SoundManager::PlaySound(const std::string  &name,
                                const std::string  &eventType,
                                const std::string  &soundEffect,
                                const RE::NiPoint3 &position,
                                float               referenceDistance,
                                float               maxDistance,
                                bool                is3D) {
        const std::size_t soundID = GenerateSoundID(name, eventType, soundEffect, position);

        if (is3D && is3DSoundEnabled) {
            if (const ALuint sourceID = Sound3DUtil::Play3DSound(
                    soundEffect, position, referenceDistance, maxDistance)) {
                activeSounds.push_back({.id                = soundID,
                                        .name              = name,
                                        .eventType         = eventType,
                                        .soundEffect       = soundEffect,
                                        .position          = position,
                                        .referenceDistance = referenceDistance,
                                        .maxDistance       = maxDistance,
                                        .is3D              = true,
                                        .descriptor        = nullptr,
                                        .sourceID          = sourceID,
                                        .startTime         = std::chrono::steady_clock::now()});
            }
        } else {
            if (const auto descriptor = std::make_shared<CustomSoundDescriptor>(soundEffect);
                descriptor->Play()) {
                activeSounds.push_back({.id          = soundID,
                                        .name        = name,
                                        .eventType   = eventType,
                                        .soundEffect = soundEffect,
                                        .position =
                                            [] {
                                                const auto *player =
                                                    RE::PlayerCharacter::GetSingleton();
                                                return player ? player->GetPosition()
                                                              : RE::NiPoint3 {0.0f, 0.0f, 0.0f};
                                            },
                                        .referenceDistance = 0,
                                        .maxDistance       = 0,
                                        .is3D              = false,
                                        .descriptor        = descriptor,
                                        .startTime         = std::chrono::steady_clock::now()});
            }
        }
    }

    bool
        SoundManager::IsSoundPlaying(std::size_t soundID) {
        return std::ranges::any_of(activeSounds, [&](const ActiveSound &sound) {
            return sound.id == soundID && sound.IsPlaying();
        });
    }

    void
        SoundManager::Update() {
        const auto     now         = std::chrono::steady_clock::now();
        constexpr auto minLifetime = std::chrono::milliseconds(300);
        // Ensures sounds are not removed immediately after being played.

        std::erase_if(activeSounds, [&](const ActiveSound &sound) {
            const bool isPlaying = sound.IsPlaying();

            if (const bool tooEarlyToRemove = now - sound.startTime < minLifetime;
                !isPlaying && !tooEarlyToRemove) {
                return true;
            }
            return false;
        });
    }

    bool
        SoundManager::Is3DSoundEnabled() {
        return is3DSoundEnabled;
    }

    bool
        SoundManager::ActiveSound::IsPlaying() const {
        if (is3D) {
            ALint state;
            alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
            return state == AL_PLAYING;
        }
        return descriptor && descriptor->IsPlaying();
    }

    const std::vector<SoundManager::ActiveSound> &
        SoundManager::GetActiveSounds() {
        return activeSounds;
    }

    std::vector<SoundManager::ActiveSound>
        SoundManager::GetSortedActiveSounds() {
        auto sounds = GetActiveSounds();

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return sounds;
        }

        std::ranges::sort(sounds, [player](const auto &a, const auto &b) {
            return player->GetPosition().GetDistance(a.GetPosition())
                 < player->GetPosition().GetDistance(b.GetPosition());
        });

        return sounds;
    }

    RE::NiPoint3
        SoundManager::ActiveSound::GetPosition() const {
        return std::visit(
            []<typename T0>(const T0 &pos) -> RE::NiPoint3 {
                using T = std::decay_t<T0>;
                if constexpr (std::is_same_v<T, RE::NiPoint3>) {
                    return pos;
                } else if constexpr (std::is_same_v<T, std::function<RE::NiPoint3()>>) {
                    return pos();
                } else {
                    return {0.0f, 0.0f, 0.0f};
                }
            },
            position);
    }

}
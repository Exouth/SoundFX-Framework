#include "SoundManager.h"
#include "Sound3D.h"

namespace SoundFX {

    std::vector<std::shared_ptr<SoundManager::ActiveSound>> SoundManager::activeSounds;
    std::atomic<bool>                                       SoundManager::is3DSoundEnabled = true;
    TaskScheduler                                           SoundManager::scheduler;
    std::mutex                                              SoundManager::activeSoundsMutex;

    SoundManager &
        SoundManager::GetInstance() {
        static SoundManager instance;
        return instance;
    }

    void
        SoundManager::Initialize() {
        if (!Sound3D::InitializeOpenAL()) {
            is3DSoundEnabled = false;
            spdlog::error("3D sound has been deactivated due to an error.");

            return;
        }

        scheduler.Start(50);
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
                                float               gain,
                                bool                isAbsoluteVolume,
                                bool                is3D) {

        if (is3D && is3DSoundEnabled) {
            Register3DSound(name,
                            eventType,
                            soundEffect,
                            position,
                            referenceDistance,
                            maxDistance,
                            gain,
                            isAbsoluteVolume);
        } else {
            Register2DSound(name, eventType, soundEffect, gain, isAbsoluteVolume);
        }
    }

    bool
        SoundManager::IsSoundPlaying(std::size_t soundID) {
        std::lock_guard lock(activeSoundsMutex);
        return std::ranges::any_of(activeSounds, [&](const auto &sound) {
            return sound && sound->id == soundID && sound->IsPlaying();
        });
    }

    void
        SoundManager::Register3DSound(const std::string  &name,
                                      const std::string  &eventType,
                                      const std::string  &soundEffect,
                                      const RE::NiPoint3 &position,
                                      float               referenceDistance,
                                      float               maxDistance,
                                      float               gain,
                                      bool                isAbsoluteVolume) {

        const std::size_t soundID = GenerateSoundID(name, eventType, soundEffect, position);

        auto sound               = std::make_shared<ActiveSound>();
        sound->id                = soundID;
        sound->name              = name;
        sound->eventType         = eventType;
        sound->soundEffect       = soundEffect;
        sound->position          = position;
        sound->referenceDistance = referenceDistance;
        sound->maxDistance       = maxDistance;
        sound->gain              = gain;
        sound->isAbsoluteVolume  = isAbsoluteVolume;
        sound->is3D              = true;
        sound->startTime         = std::chrono::steady_clock::now();

        sound->sourceID = Sound3D::Create3DSoundSource(sound);

        if (sound->sourceID != 0) {
            std::lock_guard lock(activeSoundsMutex);
            activeSounds.push_back(std::move(sound));
        } else {
            spdlog::error("3D sound '{}' : '{}' : '{}' failed to play",
                          sound->name,
                          sound->eventType,
                          sound->soundEffect);
        }
    }

    void
        SoundManager::Register2DSound(const std::string &name,
                                      const std::string &eventType,
                                      const std::string &soundEffect,
                                      float              gain,
                                      bool               isAbsoluteVolume) {
        const std::size_t soundID = GenerateSoundID(name, eventType, soundEffect, {});

        const auto descriptor = std::make_shared<CustomSoundDescriptor>(soundEffect);
        if (!descriptor->Play()) {
            spdlog::error(
                "2D sound '{}' : '{}' : '{}' failed to play", name, eventType, soundEffect);
            return;
        }

        auto sound         = std::make_shared<ActiveSound>();
        sound->id          = soundID;
        sound->name        = name;
        sound->eventType   = eventType;
        sound->soundEffect = soundEffect;
        sound->position    = [] {
            const auto *player = RE::PlayerCharacter::GetSingleton();
            return player ? player->GetPosition() : RE::NiPoint3 {0.0f, 0.0f, 0.0f};
        };
        sound->referenceDistance = 0.0f;
        sound->maxDistance       = 0.0f;
        sound->gain              = gain;
        sound->isAbsoluteVolume  = isAbsoluteVolume;
        sound->is3D              = false;
        sound->descriptor        = descriptor;
        sound->startTime         = std::chrono::steady_clock::now();

        std::lock_guard lock(activeSoundsMutex);
        activeSounds.push_back(std::move(sound));
    }

    void
        SoundManager::Update() {
        const auto     now         = std::chrono::steady_clock::now();
        constexpr auto minLifetime = std::chrono::milliseconds(300);

        const auto        *player      = RE::PlayerCharacter::GetSingleton();
        const auto        *playerObj   = player ? player->Get3D() : nullptr;
        const RE::NiPoint3 listenerPos = player ? player->GetPosition() : RE::NiPoint3 {};

        std::lock_guard lock(activeSoundsMutex);
        std::erase_if(activeSounds, [&](const std::shared_ptr<ActiveSound> &sound) {
            const bool isPlaying        = sound->IsPlaying();
            const bool tooEarlyToRemove = now - sound->startTime < minLifetime;

            if (!isPlaying && !tooEarlyToRemove) {
                if (sound->is3D && sound->sourceID != 0) {
                    alSourceStop(sound->sourceID);
                    alDeleteSources(1, &sound->sourceID);
                }
                return true;
            }

            if (sound->is3D && isPlaying) {
                Sound3D::Update3DSound(sound, listenerPos, playerObj);
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

    std::vector<std::shared_ptr<SoundManager::ActiveSound>>
        SoundManager::GetSortedActiveSounds() {
        std::lock_guard lock(activeSoundsMutex);
        auto            sounds = activeSounds;

        const auto *player = RE::PlayerCharacter::GetSingleton();
        if (!player) {
            return sounds;
        }

        std::ranges::sort(sounds, [player](const auto &a, const auto &b) {
            return player->GetPosition().GetDistance(a->GetPosition())
                 < player->GetPosition().GetDistance(b->GetPosition());
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

    void
        SoundManager::StopSound(std::size_t index) {
        std::lock_guard lock(activeSoundsMutex);

        if (index >= activeSounds.size()) {
            spdlog::warn("Invalid index {} passed to StopSound", index);
            return;
        }

        const auto &sound = activeSounds[index];

        if (sound->is3D && alIsSource(sound->sourceID)) {
            alSourceStop(sound->sourceID);
            alDeleteSources(1, &sound->sourceID);
            spdlog::info("Manually stopped 3D sound '{}' : '{}'", sound->name, sound->eventType);
        } else if (sound->descriptor) {
            sound->descriptor->Stop();
            spdlog::info("Manually stopped 2D sound '{}' : '{}'", sound->name, sound->eventType);
        }

        activeSounds.erase(
            activeSounds.begin()
            + static_cast<std::vector<std::shared_ptr<ActiveSound>>::difference_type>(index));
    }

}
#include "CustomSoundDescriptor.h"

RE::BSISoundDescriptor::BSIPlaybackCharacteristics::~BSIPlaybackCharacteristics() = default;
RE::BSISoundDescriptor::~BSISoundDescriptor()                                     = default;

namespace SoundFX {
    CustomSoundDescriptor::CustomSoundDescriptor(const std::string &filePath) {
        try {
            if (RE::BSAudioManager *audioManager = RE::BSAudioManager::GetSingleton()) {
                RE::BSResource::ID fileID;
                fileID.GenerateFromPath(filePath.c_str());
                audioManager->BuildSoundDataFromFile(soundHandle,
                                                     fileID,
                                                     /*flags=*/
                                                     0x1A,
                                                     /*priority=*/
                                                     1);
            } else {
                spdlog::error("AudioManager is null.");
            }
        } catch (const std::exception &e) {
            spdlog::error("Error creating SoundDescriptor: {}", e.what());
        }
    }

    bool
        CustomSoundDescriptor::Play() {
        if (soundHandle.IsValid()) {
            return soundHandle.Play();
        }
        spdlog::warn("SoundHandle is invalid.");
        return false;
    }

    void
        CustomSoundDescriptor::Stop() {
        if (soundHandle.IsValid()) {
            soundHandle.Stop();
        }
    }

    RE::BSSoundHandle &
        CustomSoundDescriptor::GetSoundHandle() {
        return soundHandle;
    }

    bool
        CustomSoundDescriptor::IsPlaying() const {
        return soundHandle.IsPlaying();
    }

    bool
        CustomSoundDescriptor::FadeInPlay(std::uint16_t fadeTimeMS) {
        if (soundHandle.IsValid()) {
            return soundHandle.FadeInPlay(fadeTimeMS);
        }
        return false;
    }

    bool
        CustomSoundDescriptor::FadeOutAndRelease(std::uint16_t fadeTimeMS) {
        if (soundHandle.IsValid()) {
            return soundHandle.FadeOutAndRelease(fadeTimeMS);
        }
        return false;
    }

    bool
        CustomSoundDescriptor::SetVolume(float volume) {
        if (soundHandle.IsValid()) {
            return soundHandle.SetVolume(volume);
        }
        return false;
    }

    // Take a look more at this later, maybe it can even replace OpenAL
    bool
        CustomSoundDescriptor::SetPosition(const RE::NiPoint3 position) {
        if (soundHandle.IsValid()) {
            return soundHandle.SetPosition(position);
        }
        return false;
    }

    CustomSoundDescriptor::CustomPlaybackCharacteristics &
        CustomSoundDescriptor::GetPlaybackCharacteristics() {
        return playbackCharacteristics;
    }

    void
        CustomSoundDescriptor::Unk_01() {
        // This Method is a placeholder function that is used in derived classes
        // It is currently empty and has no implementation.
    }

    void
        CustomSoundDescriptor::Unk_02() {
        // This Method is a placeholder function that is used in derived classes
        // It is currently empty and has no implementation.
    }

    std::uint8_t
        CustomSoundDescriptor::CustomPlaybackCharacteristics::GetFrequencyShift() {
        return 0;
    }

    std::uint8_t
        CustomSoundDescriptor::CustomPlaybackCharacteristics::GetFrequencyVariance() {
        return 0;
    }

    std::uint8_t
        CustomSoundDescriptor::CustomPlaybackCharacteristics::GetPriority() {
        return 1;
    }

    std::uint16_t
        CustomSoundDescriptor::CustomPlaybackCharacteristics::GetStaticAttenuation() {
        return 0;
    }

    std::uint8_t
        CustomSoundDescriptor::CustomPlaybackCharacteristics::GetDBVariance() {
        return 0;
    }

}
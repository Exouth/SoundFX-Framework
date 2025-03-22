#pragma once

namespace SoundFX {

    class CustomSoundDescriptor : public RE::BSISoundDescriptor {
      public:
        explicit CustomSoundDescriptor(const std::string &filePath);

        bool
            Play();
        void
            Stop();
        [[nodiscard]] RE::BSSoundHandle &
            GetSoundHandle();
        [[nodiscard]] bool
            IsPlaying() const;
        bool
            FadeInPlay(std::uint16_t fadeTimeMS);
        bool
            FadeOutAndRelease(std::uint16_t fadeTimeMS);
        bool
            SetVolume(float volume);
        bool
            SetPosition(RE::NiPoint3 position);

        class CustomPlaybackCharacteristics : public BSIPlaybackCharacteristics {
          public:
            ~CustomPlaybackCharacteristics() override = default;

            std::uint8_t
                GetFrequencyShift() override;
            std::uint8_t
                GetFrequencyVariance() override;
            std::uint8_t
                GetPriority() override;
            std::uint16_t
                GetStaticAttenuation() override;
            std::uint8_t
                GetDBVariance() override;
        };

        CustomPlaybackCharacteristics &
            GetPlaybackCharacteristics();

        void
            Unk_01() override;  // 01
        void
            Unk_02() override;  // 02

      private:
        RE::BSSoundHandle             soundHandle;
        CustomPlaybackCharacteristics playbackCharacteristics;
    };

}
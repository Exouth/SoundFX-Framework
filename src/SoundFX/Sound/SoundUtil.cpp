#include "SoundUtil.h"

namespace SoundFX {

    std::atomic<float> SoundUtil::masterVolume {1.0f};
    std::mutex         SoundUtil::volumeMutex;

    constexpr float EPSILON = 0.00001f;

    float
        SoundUtil::GetIngameVolumeFactor() {
        if (auto *settings = RE::INIPrefSettingCollection::GetSingleton()) {
            if (const auto *volumeSetting = settings->GetSetting("fAudioMasterVolume:AudioMenu")) {
                return volumeSetting->data.f;
            }
        }
        return 1.0f;
    }

    bool
        SoundUtil::CheckAndUpdateMasterVolume() {
        std::lock_guard lock(volumeMutex);
        float           newMasterVolume = GetIngameVolumeFactor();
        if (std::abs(newMasterVolume - masterVolume.load()) > EPSILON) {
            masterVolume.store(newMasterVolume);
            return true;
        }
        return false;
    }

    float
        SoundUtil::CalculateFinalVolume(float gain, float volume, bool isAbsoluteVolume) {
        CheckAndUpdateMasterVolume();

        const float currentMasterVolume = masterVolume.load();

        if (std::abs(gain - -1.0f) < EPSILON) {
            return std::clamp(currentMasterVolume * volume, 0.0f, 1.0f);
        }

        float finalVolume = gain * currentMasterVolume * volume;

        if (isAbsoluteVolume) {
            finalVolume = gain * volume;
        }

        return std::clamp(finalVolume, 0.0f, 1.0f);
    }

}
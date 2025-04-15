#pragma once

namespace SoundFX {
    class SoundUtil {
      public:
        static bool
            CheckAndUpdateMasterVolume();

        static float
            CalculateFinalVolume(float gain, float volume, bool isAbsoluteVolume);

        static float
            GetIngameVolumeFactor();

      private:
        static std::atomic<float> masterVolume;
        static std::mutex         volumeMutex;
    };
}
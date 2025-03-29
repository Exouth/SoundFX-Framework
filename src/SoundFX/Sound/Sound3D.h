#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace SoundFX {

    class Sound3D {
      public:
        static bool
            InitializeOpenAL();

        static ALuint
            Play3DSound(const std::string  &filePath,
                        const RE::NiPoint3 &worldSourcePos,
                        float               referenceDistance = 100.0f,
                        float               maxDistance       = 1000.0f,
                        float               gain              = 1.0f,
                        bool                isAbsoluteVolume  = true,
                        float               rolloffFactor     = 1.0f,
                        float               minGain           = 0.0f);

        static void
            Shutdown();

      private:
        static RE::NiPoint3
            GetForwardVector(const RE::NiAVObject *object);

        static ALCdevice *
            InitializeDevice();
        static ALCcontext *
            InitializeContext(ALCdevice *device);
        static ALuint
            LoadAudioBuffer(const std::string &filePath);

        static bool
            InitializeSharedContext();

        static inline ALCdevice                              *sharedDevice  = nullptr;
        static inline ALCcontext                             *sharedContext = nullptr;
        static inline std::unordered_map<std::string, ALuint> bufferCache;
        static inline std::mutex                              bufferCacheMutex;
        static inline std::atomic<float>                      cachedVolume = 1.0f;
    };

}
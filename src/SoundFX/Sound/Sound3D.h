#pragma once

#include "SoundManager.h"
#include <AL/al.h>
#include <AL/alc.h>

namespace SoundFX {

    class Sound3D {
      public:
        static bool
            InitializeOpenAL();

        static ALuint
            Create3DSoundSource(const std::shared_ptr<SoundManager::ActiveSound> &sound);

        static void
            Update3DSound(const std::shared_ptr<SoundManager::ActiveSound> &sound,
                          const RE::NiPoint3                               &listenerPos,
                          const RE::NiAVObject                             *listenerObj);

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
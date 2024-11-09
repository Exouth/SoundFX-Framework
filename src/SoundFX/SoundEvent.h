#pragma once

#include "SoundUtil.h"
#include <string>

namespace SoundFX {

    class SoundEvent {
      public:
        float       chance;
        int         repeatFrequency;
        float       volume;
        std::string soundEffect;

        virtual bool
            play() {
            return SoundFX::PlayCustomSoundAsDescriptor(soundEffect);
        }

        virtual void
            stop() {
        }

        virtual ~SoundEvent() = default;
    };

}
#include "SoundUtil.h"
#include "CustomSoundDescriptor.h"
#include "spdlog/spdlog.h"

namespace SoundFX {

    bool
        PlayCustomSoundAsDescriptor(const std::string &soundFilePath) {
        try {
            auto soundDescriptor = std::make_shared<CustomSoundDescriptor>(soundFilePath);
            if (soundDescriptor->Play()) {
                return true;
            }
            spdlog::info("Sound finished, returning to event.");
            return false;
        } catch (const std::exception &e) {
            spdlog::error("Error in PlayCustomSoundAsDescriptor: {}", e.what());
            return false;
        }
    }

}
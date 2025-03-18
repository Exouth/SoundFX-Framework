#include "Sound3DUtil.h"

namespace {
    ALCdevice  *sharedDevice  = nullptr;
    ALCcontext *sharedContext = nullptr;

    std::unordered_map<std::string, ALuint> bufferCache;
}

namespace SoundFX {

    bool
        Sound3DUtil::InitializeOpenAL() {
        if (!InitializeSharedContext()) {
            spdlog::error("OpenAL could not be initialized.");
            return false;
        }
        return true;
    }

    RE::NiPoint3
        Sound3DUtil::GetForwardVector(const RE::NiAVObject *object) {
        if (!object) {
            spdlog::error("GetForwardVector: object is null");
            return {0.0f, 0.0f, 1.0f};  // Fallback
        }

        auto rotationMatrix = object->world.rotate;

        return {
            rotationMatrix.entry[0][1],  // x
            rotationMatrix.entry[1][1],  // y
            rotationMatrix.entry[2][1]   // z
        };
    }

    ALCdevice *
        Sound3DUtil::InitializeDevice() {
        ALCdevice *device = alcOpenDevice(nullptr);
        if (!device) {
            spdlog::error("Failed to open OpenAL device.");
        }
        return device;
    }

    ALCcontext *
        Sound3DUtil::InitializeContext(ALCdevice *device) {
        if (!device) {
            return nullptr;
        }

        ALCcontext *context = alcCreateContext(device, nullptr);
        if (!context || alcMakeContextCurrent(context) == ALC_FALSE) {
            spdlog::error("Failed to create or set OpenAL context.");
            alcDestroyContext(context);
            alcCloseDevice(device);
            return nullptr;
        }

        return context;
    }

    ALuint
        Sound3DUtil::LoadAudioBuffer(const std::string &filePath) {

        if (bufferCache.contains(filePath)) {
            return bufferCache[filePath];
        }

        const std::string fullPath = "Data/" + filePath;

        SF_INFO  fileInfo {};
        SNDFILE *sndFile = sf_open(fullPath.c_str(), SFM_READ, &fileInfo);
        if (!sndFile) {
            spdlog::error("Failed to open audio file: {}", filePath);
            return 0;
        }

        if (fileInfo.channels > 2) {
            spdlog::error("Unsupported audio format (only mono or stereo supported).");
            sf_close(sndFile);
            return 0;
        }

        std::vector<short> samples(fileInfo.frames * fileInfo.channels);

        if (samples.size() > static_cast<size_t>(std::numeric_limits<sf_count_t>::max())) {
            spdlog::error("Sample size exceeds the allowed range for sf_count_t!");
            return 0;
        }

        sf_read_short(sndFile, samples.data(), static_cast<sf_count_t>(samples.size()));
        sf_close(sndFile);

        ALuint buffer;
        alGenBuffers(1, &buffer);

        const ALenum format = fileInfo.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

        if (samples.size() * sizeof(short)
            > static_cast<size_t>(std::numeric_limits<ALsizei>::max())) {
            spdlog::error("Audio buffer size exceeds the allowed range for OpenAL!");
            return 0;
        }

        alBufferData(buffer,
                     format,
                     samples.data(),
                     static_cast<ALsizei>(samples.size() * sizeof(short)),
                     fileInfo.samplerate);

        bufferCache[filePath] = buffer;

        return buffer;
    }

    bool
        Sound3DUtil::InitializeSharedContext() {
        if (!sharedDevice) {
            sharedDevice = InitializeDevice();
            if (!sharedDevice) {
                spdlog::error("Failed to initialize shared OpenAL device.");
                return false;
            }
        }
        if (!sharedContext) {
            sharedContext = InitializeContext(sharedDevice);
            if (!sharedContext) {
                spdlog::error("Failed to initialize shared OpenAL context.");
                alcCloseDevice(sharedDevice);
                sharedDevice = nullptr;
                return false;
            }
        }
        return true;
    }

    ALuint
        Sound3DUtil::Play3DSound(const std::string  &filePath,
                                 const RE::NiPoint3 &worldSourcePos,
                                 float               referenceDistance,
                                 float               maxDistance,
                                 float               rolloffFactor,
                                 float               gain,
                                 float               minGain) {

        const ALuint buffer = LoadAudioBuffer(filePath);
        if (buffer == 0) {
            spdlog::error("Failed to load audio buffer.");
            return 0;
        }

        ALuint source;
        alGenSources(1, &source);
        alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
        alSourcef(source, AL_REFERENCE_DISTANCE, referenceDistance);
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);
        alSourcef(source, AL_GAIN, gain);
        alSourcef(source, AL_MIN_GAIN, minGain);

        alSourcePlay(source);

        std::thread([source, worldSourcePos] {
            ALint state = AL_PLAYING;

            RE::NiPoint3 initialPlayerPosition;
            bool         isInitialized = false;

            while (state == AL_PLAYING) {
                if (const auto *player = RE::PlayerCharacter::GetSingleton();
                    player && player->Get3D()) {
                    const RE::NiPoint3 currentPlayerPosition = player->GetPosition();

                    if (!isInitialized) {
                        initialPlayerPosition = currentPlayerPosition;
                        isInitialized         = true;
                    }

                    const RE::NiPoint3 relativeSourcePos = {
                        worldSourcePos.x - (currentPlayerPosition.x - initialPlayerPosition.x),
                        worldSourcePos.y - (currentPlayerPosition.y - initialPlayerPosition.y),
                        worldSourcePos.z - (currentPlayerPosition.z - initialPlayerPosition.z)};

                    const RE::NiPoint3     forwardVector = GetForwardVector(player->Get3D());
                    constexpr RE::NiPoint3 upVector      = {0.0f, 0.0f, 1.0f};

                    constexpr ALfloat listenerPos[3] = {0.0f, 0.0f, 0.0f};
                    const ALfloat     listenerOri[6] = {forwardVector.x,
                                                        forwardVector.y,
                                                        forwardVector.z,
                                                        upVector.x,
                                                        upVector.y,
                                                        upVector.z};

                    const ALfloat sourcePos[3] = {
                        relativeSourcePos.x, relativeSourcePos.y, relativeSourcePos.z};
                    alSourcefv(source, AL_POSITION, sourcePos);
                    alListenerfv(AL_POSITION, listenerPos);
                    alListenerfv(AL_ORIENTATION, listenerOri);
                }

                alGetSourcei(source, AL_SOURCE_STATE, &state);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            alDeleteSources(1, &source);
        }).detach();

        return source;
    }

    void
        Sound3DUtil::Shutdown() {
        for (auto &buffer : bufferCache | std::views::values) {
            alDeleteBuffers(1, &buffer);
        }
        bufferCache.clear();

        if (sharedContext) {
            alcDestroyContext(sharedContext);
            sharedContext = nullptr;
        }

        if (sharedDevice) {
            alcCloseDevice(sharedDevice);
            sharedDevice = nullptr;
        }
    }

}
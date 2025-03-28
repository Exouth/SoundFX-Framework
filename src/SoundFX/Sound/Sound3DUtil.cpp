#include "Sound3DUtil.h"
#include <ranges>
#include <sndfile.h>

namespace SoundFX {

    bool
        Sound3DUtil::InitializeOpenAL() {
        if (!InitializeSharedContext()) {
            spdlog::critical("OpenAL could not be initialized.");
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

        const auto rotationMatrix = object->world.rotate;

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
            spdlog::critical("Failed to open OpenAL device.");
        }
        return device;
    }

    ALCcontext *
        Sound3DUtil::InitializeContext(ALCdevice *device) {
        if (!device) {
            return nullptr;
        }

        const ALCint attributes[] = {ALC_MONO_SOURCES,
                                     4000,  // Maximum number of mono sources
                                     ALC_STEREO_SOURCES,
                                     4000,  // Maximum number of stereo sources
                                     0};

        ALCcontext *context = alcCreateContext(device, attributes);
        if (!context || alcMakeContextCurrent(context) == ALC_FALSE) {
            spdlog::critical("Failed to create or set OpenAL context.");
            alcDestroyContext(context);
            alcCloseDevice(device);
            return nullptr;
        }

        return context;
    }

    ALuint
        Sound3DUtil::LoadAudioBuffer(const std::string &filePath) {
        {
            std::lock_guard lock(bufferCacheMutex);
            const auto      it = bufferCache.find(filePath);
            if (it != bufferCache.end()) {
                return it->second;
            }
        }

        const std::filesystem::path fullPath = std::filesystem::path("Data") / filePath;

        SF_INFO  fileInfo {};
        SNDFILE *sndFile = sf_open(fullPath.string().c_str(), SFM_READ, &fileInfo);
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
        if (sf_read_short(sndFile, samples.data(), static_cast<sf_count_t>(samples.size()))
            < samples.size()) {
            spdlog::error("Failed to read audio samples from file: {}", filePath);
            sf_close(sndFile);
            return 0;
        }
        sf_close(sndFile);

        ALuint buffer;
        alGenBuffers(1, &buffer);
        const ALenum format = fileInfo.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
        alBufferData(buffer,
                     format,
                     samples.data(),
                     static_cast<ALsizei>(samples.size() * sizeof(short)),
                     fileInfo.samplerate);
        if (alGetError() != AL_NO_ERROR) {
            spdlog::error("Failed to fill OpenAL buffer with audio data.");
            alDeleteBuffers(1, &buffer);
            return 0;
        }

        {
            std::lock_guard lock(bufferCacheMutex);
            bufferCache[filePath] = buffer;
        }

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
        if (alGetError() != AL_NO_ERROR) {
            spdlog::error("Failed to generate OpenAL source.");
            return 0;
        }

        alSourcei(source, AL_BUFFER, static_cast<ALint>(buffer));
        alSourcef(source, AL_REFERENCE_DISTANCE, referenceDistance);
        alSourcef(source, AL_MAX_DISTANCE, maxDistance);
        alSourcef(source, AL_ROLLOFF_FACTOR, rolloffFactor);
        alSourcef(source, AL_MIN_GAIN, minGain);

        alSourcePlay(source);
        if (alGetError() != AL_NO_ERROR) {
            spdlog::error("Failed to play OpenAL source.");
            alDeleteSources(1, &source);
            return 0;
        }

        std::thread([source, worldSourcePos, maxDistance, gain] {
            ALint state = AL_PLAYING;

            bool isInitialized = false;

            while (state == AL_PLAYING) {
                if (const auto *player = RE::PlayerCharacter::GetSingleton();
                    player && player->Get3D()) {
                    const RE::NiPoint3 currentPlayerPosition = player->GetPosition();

                    if (!isInitialized) {
                        isInitialized = true;
                    }

                    const RE::NiPoint3 relativeSourcePos = {
                        worldSourcePos.x - currentPlayerPosition.x,
                        worldSourcePos.y - currentPlayerPosition.y,
                        worldSourcePos.z - currentPlayerPosition.z};

                    const float distance = relativeSourcePos.Length();
                    float       volume   = 1.0f - distance / maxDistance;
                    volume               = std::clamp(volume, 0.0f, 1.0f);

                    const float ingameVolume = GetIngameVolumeFactor();
                    const float finalVolume  = volume * gain * ingameVolume;

                    float currentGain;
                    alGetSourcef(source, AL_GAIN, &currentGain);
                    if (std::abs(currentGain - finalVolume) > 0.01f) {
                        alSourcef(source, AL_GAIN, finalVolume);
                    }

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
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            alDeleteSources(1, &source);
            if (alGetError() != AL_NO_ERROR) {
                spdlog::critical("Failed to delete OpenAL source: {}", source);
            }
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

    float
        Sound3DUtil::GetIngameVolumeFactor() {
        if (auto *settings = RE::INIPrefSettingCollection::GetSingleton()) {
            if (const auto *volumeSetting = settings->GetSetting("fAudioMasterVolume:AudioMenu")) {
                const float     newVolume = volumeSetting->data.f;
                constexpr float epsilon   = 0.0001f;
                if (std::abs(newVolume - cachedVolume.load()) > epsilon) {
                    cachedVolume.store(newVolume);
                }
                return cachedVolume.load();
            }
        }
        return 1.0f;
    }
}
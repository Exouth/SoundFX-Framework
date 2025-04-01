#include "Sound3D.h"
#include "SoundManager.h"
#include "SoundUtil.h"
#include <ranges>
#include <sndfile.h>

namespace SoundFX {

    bool
        Sound3D::InitializeOpenAL() {
        if (!InitializeSharedContext()) {
            spdlog::critical("OpenAL could not be initialized.");
            return false;
        }
        return true;
    }

    RE::NiPoint3
        Sound3D::GetForwardVector(const RE::NiAVObject *object) {
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
        Sound3D::InitializeDevice() {
        ALCdevice *device = alcOpenDevice(nullptr);
        if (!device) {
            spdlog::critical("Failed to open OpenAL device.");
        }
        return device;
    }

    ALCcontext *
        Sound3D::InitializeContext(ALCdevice *device) {
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
        Sound3D::LoadAudioBuffer(const std::string &filePath) {
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
        const sf_count_t   samplesRead =
            sf_read_short(sndFile, samples.data(), static_cast<sf_count_t>(samples.size()));

        if (samplesRead < static_cast<sf_count_t>(samples.size())) {
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
        Sound3D::InitializeSharedContext() {
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
        Sound3D::Create3DSoundSource(const std::shared_ptr<SoundManager::ActiveSound> &sound) {
        const ALuint buffer = LoadAudioBuffer(sound->soundEffect);
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
        alSourcef(source, AL_REFERENCE_DISTANCE, sound->referenceDistance);
        alSourcef(source, AL_MAX_DISTANCE, sound->maxDistance);
        alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
        alSourcef(source, AL_MIN_GAIN, 0.0f);

        const float initialFinalGain =
            SoundUtil::CalculateFinalVolume(sound->gain, 1.0f, sound->isAbsoluteVolume);

        alSourcef(source, AL_GAIN, initialFinalGain);

        const RE::NiPoint3 pos          = sound->GetPosition();
        const ALfloat      sourcePos[3] = {pos.x, pos.y, pos.z};
        alSourcefv(source, AL_POSITION, sourcePos);

        alSourcePlay(source);
        if (alGetError() != AL_NO_ERROR) {
            spdlog::error("Failed to play OpenAL source.");
            alDeleteSources(1, &source);
            return 0;
        }

        return source;
    }

    void
        Sound3D::Update3DSound(const std::shared_ptr<SoundManager::ActiveSound> &sound,
                               const RE::NiPoint3                               &listenerPos,
                               const RE::NiAVObject                             *listenerObj) {
        if (!sound || !sound->is3D || sound->sourceID == 0 || !alIsSource(sound->sourceID)) {
            return;
        }

        const RE::NiPoint3 worldSourcePos = sound->GetPosition();
        const RE::NiPoint3 relPos         = {worldSourcePos.x - listenerPos.x,
                                             worldSourcePos.y - listenerPos.y,
                                             worldSourcePos.z - listenerPos.z};

        const float distance = relPos.Length();
        float       volume   = 1.0f - distance / sound->maxDistance;
        volume               = std::clamp(volume, 0.0f, 1.0f);

        const float finalGain =
            SoundUtil::CalculateFinalVolume(sound->gain, volume, sound->isAbsoluteVolume);

        float currentGain = 0.0f;
        alGetSourcef(sound->sourceID, AL_GAIN, &currentGain);
        if (std::abs(currentGain - finalGain) > 0.01f) {
            alSourcef(sound->sourceID, AL_GAIN, finalGain);
        }

        const ALfloat sourcePos[3] = {relPos.x, relPos.y, relPos.z};
        alSourcefv(sound->sourceID, AL_POSITION, sourcePos);

        if (listenerObj) {
            const RE::NiPoint3     forward = GetForwardVector(listenerObj);
            constexpr RE::NiPoint3 up      = {0.0f, 0.0f, 1.0f};
            const ALfloat listenerOri[6]   = {forward.x, forward.y, forward.z, up.x, up.y, up.z};

            constexpr ALfloat zeroListenerPos[3] = {0.0f, 0.0f, 0.0f};
            alListenerfv(AL_POSITION, zeroListenerPos);
            alListenerfv(AL_ORIENTATION, listenerOri);
        }
    }

    void
        Sound3D::Shutdown() {
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
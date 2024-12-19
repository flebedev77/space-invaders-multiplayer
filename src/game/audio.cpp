#include "audio.h"
#include "assets/audio/hit.h"
#include "assets/audio/zap.h"
#include "assets/audio/thud.h"
#include "assets/audio/shoot.h"

namespace audio
{
    AudioData sounds[];

    AudioData currentData;

    SDL_AudioDeviceID device = 0;

    AudioData LoadWAVFromMemory(unsigned char wav_data[], unsigned int wav_len)
    {
        AudioData d;
        SDL_IOStream *io = SDL_IOFromMem(wav_data, wav_len);
        if (io == nullptr)
        {
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to create IO, wav from memory!");
        }

        if (!SDL_LoadWAV_IO(io, true, &d.spec, &d.buffer, &d.bufferLength))
        {
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Falied loading wav!");
        }

        

        return d;
    }

    void createStream(SDL_AudioStream *stream, SDL_AudioSpec* spec)
    {
        stream = SDL_CreateAudioStream(spec, NULL);
        if (!stream)
        {
            SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        }
        else if (!SDL_BindAudioStream(device, stream))
        { 
            SDL_Log("Failed to bind stream to device: %s", SDL_GetError());
        }
    }

    void LoadAudios()
    {
        device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        if (device == 0)
        {
            SDL_Log("Couldn't open audio device: %s", SDL_GetError());
        }

        sounds[Sound::hitSound] = LoadWAVFromMemory(hit_wav, hit_wav_len);
        sounds[Sound::zapSound] = LoadWAVFromMemory(zap_wav, zap_wav_len);
        sounds[Sound::thudSound] = LoadWAVFromMemory(thud_wav, thud_wav_len);
        sounds[Sound::shootSound] = LoadWAVFromMemory(shoot_wav, shoot_wav_len);

        for(size_t i = 0; i < streamAmt; i++)
        {
            createStream(streams[i], &sounds[0].spec);
        }
    }

    SDL_AudioStream* findEmptyStream(SDL_AudioSpec* spec)
    {
        int lowestStreamData = SDL_MAX_SINT32;
        SDL_AudioStream* lowestStream = nullptr;
        for(size_t i = 0; i < streamAmt; i++)
        {
            SDL_AudioStream* stream = streams[i];

            int streamData = SDL_GetAudioStreamAvailable(stream);
            if (streamData < lowestStreamData)
            {
                lowestStreamData = streamData;
                lowestStream = stream;
            }
        }
        return lowestStream;
    }

    void PlaySound(uint32_t soundId)
    {
        AudioData& data = sounds[soundId];
        SDL_AudioStream* stream = findEmptyStream(&data.spec);
        if (stream != nullptr)
        {
            SDL_PutAudioStreamData(stream, data.buffer, data.bufferLength);
        }
    }
}
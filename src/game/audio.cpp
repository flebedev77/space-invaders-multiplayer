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


        d.stream = SDL_CreateAudioStream(&d.spec, NULL);
        if (!d.stream)
        {
            SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        }
        else if (!SDL_BindAudioStream(device, d.stream))
        { /* once bound, it'll start playing when there is data available! */
            SDL_Log("Failed to bind stream to device: %s", SDL_GetError());
        }

        return d;
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
    }

    void PlaySound(AudioData &data)
    {
        if (data.stream != nullptr)
        {
            SDL_PutAudioStreamData(data.stream, data.buffer, data.bufferLength);
        }
    }
}
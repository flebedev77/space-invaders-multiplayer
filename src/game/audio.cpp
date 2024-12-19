#include "audio.h"
#include "assets/audio/hit.h"

namespace audio
{
    AudioData hitSound;
    AudioData currentData;

    AudioData *LoadWAVFromMemory(unsigned char wav_data[], unsigned int wav_len)
    {
        SDL_IOStream *io = SDL_IOFromMem(wav_data, wav_len);
        if (io == nullptr)
        {
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Failed to create IO, wav from memory!");
            return nullptr;
        }

        if (!SDL_LoadWAV_IO(io, true, &currentData.spec, &currentData.buffer, &currentData.bufferLength))
        {
            SDL_LogError(SDL_LOG_PRIORITY_ERROR, "Falied loading wav!");
            return nullptr;
        }

        return &currentData;
    }

    void LoadAudios()
    {
        hitSound = *LoadWAVFromMemory(hit_wav, hit_wav_len);
    }

    void PlaySound(AudioData &data, SDL_AudioStream *stream)
    {
        if (stream != nullptr)
        {
            SDL_ClearAudioStream(stream);
            SDL_PutAudioStreamData(stream, data.buffer, data.bufferLength);
        }
    }
}
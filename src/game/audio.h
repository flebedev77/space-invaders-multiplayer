#pragma once
#include <SDL3/SDL.h>

namespace audio
{
    struct AudioData
    {
        uint8_t *buffer;
        uint32_t bufferLength;
        SDL_AudioSpec spec;
    };

    extern AudioData hitSound;
    extern AudioData currentData;

    AudioData *LoadWAVFromMemory(unsigned char wav_data[], unsigned int wav_data_len);
    void LoadAudios();

    void PlaySound(AudioData& data, SDL_AudioStream* stream);
};
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

    enum Sound
    {
        hitSound,
        zapSound,
        thudSound,
        shootSound
    };

    extern AudioData sounds[4];
    const size_t streamAmt = 16;
    extern SDL_AudioStream *streams[streamAmt];

    extern AudioData currentData;

    extern SDL_AudioDeviceID device;

    AudioData LoadWAVFromMemory(unsigned char wav_data[], unsigned int wav_data_len);
    void LoadAudios();

    void PlaySound(uint32_t soundId);
};
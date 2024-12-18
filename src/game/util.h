#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include <string>
#include <cstring>
#include <random>
#include <ctime>
#include <random>

namespace utils
{
    extern uint32_t cameraShakeDelay;
    extern uint32_t cameraShakeRate;

    extern bool cameraShakeRunning;
    extern int iterations;
    extern int intensity;

    bool aabb(SDL_FRect* a, SDL_FRect* b);

    SDL_Surface* loadEmbeddedBMP(SDL_Renderer* renderer, unsigned char bmp[], unsigned int bmpLength);
    float lerp(float a, float b, float t);

    std::string formatZerosString(uint16_t digitAmount, uint16_t &value);

    void cameraShake(SDL_Rect& camera, int iterations = 20, int intensity = 20);
    void cameraUpdate(SDL_Renderer* renderer, SDL_Rect& smoothCamera, SDL_Rect& camera, uint32_t deltaTime, uint32_t frameIndex, float cameraSmoothness);
};
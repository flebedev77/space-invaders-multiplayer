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
    uint32_t cameraShakeDelay = 0;
    uint32_t cameraShakeRate = 100;

    bool cameraShakeRunning = false;
    int iterations = 5;
    int intensity = 2;

    bool aabb(SDL_FRect* a, SDL_FRect* b);

    SDL_Surface* loadEmbeddedBMP(SDL_Renderer* renderer, unsigned char bmp[], unsigned int bmpLength);
    float lerp(float a, float b, float t);

    std::string formatZerosString(uint16_t digitAmount, uint16_t &value);

    void cameraShake(SDL_Rect& camera, int iterations = 5, int intensity = 2);
    void cameraUpdate(SDL_Rect& camera, uint32_t deltaTime);
};
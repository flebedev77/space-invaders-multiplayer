#pragma once
#include <SDL3/SDL.h>
#include <iostream>
#include <ostream>

namespace utils
{
    bool aabb(SDL_FRect* a, SDL_FRect* b);

    SDL_Surface* loadEmbeddedBMP(SDL_Renderer* renderer, unsigned char bmp[], unsigned int bmpLength);
    float lerp(float a, float b, float t);
};
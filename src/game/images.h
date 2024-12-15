#pragma once
#include <SDL3/SDL.h>
#include "util.h"
#include "assets/alien.h"
#include "assets/bullet.h"
#include "assets/player.h"

namespace images 
{
    SDL_Surface* alien;
    SDL_Surface* bullet;
    SDL_Surface* player;

    void LoadImages(SDL_Renderer* renderer);
    void UnloadImages();
};
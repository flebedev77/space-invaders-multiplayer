#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "alien.h"

namespace alienmgr 
{
    void SpawnAliens(int amt_h, int amt_v, std::vector<Alien>& aliens);
    void UpdateAliens(SDL_Renderer* renderer, std::vector<Alien>& aliens, int windowWidth, uint32_t deltaTime);
};
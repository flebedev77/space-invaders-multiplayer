#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include <cstdlib>
#include <random>
#include <ctime>
#include "alien.h"
#include "block.h"
#include "player.h"

namespace alienmgr 
{
    void SpawnAliens(int amt_h, int amt_v, std::vector<Alien>& aliens);
    void UpdateAliens(SDL_Renderer* renderer, std::vector<Alien>& aliens, std::vector<Block>& blocks, int windowWidth, uint32_t deltaTime, Player& player, float moveDownAmount);
};
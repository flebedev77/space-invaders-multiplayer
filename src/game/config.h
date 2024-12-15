#pragma once
#include <vector>
#include "color.h"
#include "player.h"
#include "block.h"

namespace config {
    constexpr int windowWidth = 400;
    constexpr int windowHeight = 500;
    Color backgroundColor{30.f, 30.f, 30.f};

    Player player = Player{};
    std::vector<Block> blocks;

    constexpr float blockWidth = 20;
    constexpr float blockHeight = 20;
    constexpr int blockMaxHealth = 100;
    constexpr int wallHeightFromFloor = 100;

    constexpr int bulletDamage = 25;
    
    uint32_t prevTime = SDL_GetTicks();
    uint32_t deltaTime = 0;

    namespace keys {
        bool left = 0;
        bool right = 0;
        bool shoot = 0;
    };
};
#pragma once
#include "color.h"
#include "player.h"

namespace config {
    constexpr int windowWidth = 400;
    constexpr int windowHeight = 500;
    Color backgroundColor{30.f, 30.f, 30.f};

    Player player = Player{};

    uint32_t prevTime = SDL_GetTicks();
    uint32_t deltaTime = 0;

    namespace keys {
        bool left = 0;
        bool right = 0;
    };
};
#pragma once
#include <vector>
#include <cstdint>
#include "color.h"
#include "player.h"
#include "block.h"
#include "alien.h"
#include "particle.h"

namespace config
{
    constexpr int windowWidth = 400;
    constexpr int windowHeight = 500;
    Color backgroundColor{30.f, 30.f, 30.f};

    Player player = Player{};
    uint16_t playerKills = 0;
    std::vector<Block> blocks;
    std::vector<Alien> aliens;
    int alien_amt_h = 3;
    int alien_amt_v = 2;
    float alienMoveDownAmount = 5.f;

    constexpr size_t stars_amt = 64;
    Particle stars[stars_amt];
    std::vector<Particle> global_particles;

    constexpr float blockWidth = 20.f;
    constexpr float blockHeight = 20.f;

    constexpr int blockMaxHealth = 100;
    constexpr int wallHeightFromFloor = 100;

    constexpr int bulletDamage = 25;

    constexpr float healthbarHeight = 15.f;
    constexpr float healthbarWidth = float(windowWidth);

    bool hasStarted = false;
    bool isGameOver = false;
    bool isSwitchingNextStage = false;

    uint16_t displayDigitAmt = 4;
    uint16_t stageNum = 1;
    uint16_t bestStageNum = 1;
    uint32_t switchingNextStageDelay = 0;
    uint32_t switchingNextStageRate = 1500;

    uint32_t prevTime = SDL_GetTicks();
    uint32_t deltaTime = 0;

    uint32_t shootDelay = 0;
    uint32_t shootRate = 70; // 70ms

    int menuScreenColorIndex = 0;
    uint32_t menuScreenColorChangeDelay = 0;
    uint32_t menuScreenColorChangeRate = 1000;

    namespace keys
    {
        bool left = 0;
        bool right = 0;
        bool shoot = 0;
    };
};
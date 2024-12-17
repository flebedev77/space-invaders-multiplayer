#pragma once
#include <vector>
#include <cstdint>
#include "vector2.h"
#include "color.h"
#include "player.h"
#include "block.h"
#include "particle.h"
#include "controls.h"

namespace config
{
    constexpr int windowWidth = 400;
    constexpr int windowHeight = 500;
    Color backgroundColor{30.f, 30.f, 30.f};

    size_t playerAmt = 2;
    Player players[] = {Player{}, Player{}};
    Vector2 playerSpawnPositions[] = {
        Vector2{float(windowWidth / 2), 40.f},                    // top player
        Vector2{float(windowWidth / 2), float(windowHeight - 20)} // bottom player
    };
    std::vector<Block> blocks;

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
    uint gameOverRestartDelay = 0;
    uint gameOverRestartRate = 1000;
    uint8_t gameOverPlayerIndex = 0;

    uint16_t displayDigitAmt = 4;

    uint32_t prevTime = SDL_GetTicks();
    uint32_t deltaTime = 0;

    uint32_t shootRate = 70; // 70ms

    int menuScreenColorIndex = 0;
    uint32_t menuScreenColorChangeDelay = 0;
    uint32_t menuScreenColorChangeRate = 1000;

    Controls keys[] = {Controls{}, Controls{}};
};
#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "vector2.h"
#include "bullet.h"
#include "drawutils.h"
#include "images.h"

struct Player {
    Vector2 position{0, 0}; 
    float radius = 0.f;
    float speed = 0.5f;
    float width, height;
    std::vector<Bullet> bullets;

    int maxHealth = 100;
    int health = 100;
    float playerSmoothHealth = 100.f;

    uint32_t animationDelay = 400;
    uint32_t animationRate = 600;

    uint32_t shootDelay = 0;

    bool selectedTexture = 0;
    bool lookingDown = 0;
    bool prevShotLeft = false;

    Player(Vector2 pos = Vector2{}, float r = 20.f): position(Vector2{}), radius(r) {
        position.x = pos.x;
        position.y = pos.y;
    };

    void Draw(SDL_Renderer* renderer, uint32_t deltaTime);
    void Move(int dir, uint32_t deltaTime);
    void Shoot();
};
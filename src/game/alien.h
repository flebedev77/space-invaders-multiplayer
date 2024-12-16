#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "vector2.h"
#include "bullet.h"
#include "block.h"
#include "images.h"
#include "util.h"

struct Alien
{
    Vector2 position{};
    std::vector<Bullet> bullets;
    float width = 0.f; 
    float height = 0.f;

    int maxHealth = 100;
    int health = 100;

    int bulletDamage = 25;

    uint32_t shootDelay = 0;
    uint32_t shootRate = 3000;

    Alien(float x, float y, float w, float h): width(w), height(h) {
        position.x = x;
        position.y = y;
    } 

    void Draw(SDL_Renderer* renderer);
    void Update_Shoot(SDL_Renderer* renderer, uint32_t deltaTime, std::vector<Block>& blocks);
};
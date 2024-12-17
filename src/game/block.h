#pragma once
#include <SDL3/SDL.h>
#include "vector2.h"
#include "color.h"
#include "images.h"

struct Block
{
    Vector2 position{};
    float width = 0.f;
    float height = 0.f;
    int health = 10;
    bool animated = true;
    bool frame = 0;

    uint32_t animationFrameDelay = 0;
    uint32_t animationFrameRate = 500;

    Block(float x, float y, float w, float h, int blockHealth, uint32_t animationDelay) : width(w), height(h)
    {
        animationFrameDelay = animationDelay;
        health = blockHealth;
        position.x = x;
        position.y = y;
    }

    void Draw(SDL_Renderer *renderer, uint32_t deltaTime);
};
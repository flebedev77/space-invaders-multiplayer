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
    int health = 100;

    Block(float x, float y, float w, float h, int blockHealth) : width(w), height(h)
    {
        health = blockHealth;
        position.x = x;
        position.y = y;
    }

    void Draw(SDL_Renderer *renderer);
};
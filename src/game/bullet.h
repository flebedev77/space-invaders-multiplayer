#pragma once
#include <SDL3/SDL.h>
#include "vector2.h"
#include "color.h"

struct Bullet {
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    Color color;

    Bullet(float x = 0, float y = 0):
        position(Vector2{}),
        velocity(Vector2{}),
        width(5.f),
        height(13.f),
        color(Color{0.9f, 0.8f, 0.1f})
    {
        position.x = x;
        position.y = y;
    }

    void Draw(SDL_Renderer* renderer);
    void Update(uint32_t deltaTime);
};
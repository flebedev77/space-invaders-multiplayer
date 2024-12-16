#pragma once
#include <SDL3/SDL.h>
#include "vector2.h"
#include "color.h"

struct Particle
{
    Vector2 position{};
    Vector2 velocity{0, 0};
    uint32_t lifetime = 0;
    uint32_t maxLifetime = 1500;

    float width = 1.f;
    float height = 1.f;

    Color color{0.f, 0.f, 0.f};

    Particle(float x = 0.f, float y = 0.f) : lifetime(0)
    {
        position.x = x;
        position.y = y;
    }

    void Draw(SDL_Renderer* renderer);
    void Update(uint32_t deltaTime);
};
#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <random>
#include <ctime>
#include "vector2.h"
#include "color.h"
#include "images.h"
#include "particle.h"

struct Bullet
{
    Vector2 position;
    Vector2 velocity;
    float width;
    float height;
    Color color;

    std::vector<Particle> particles;

    Bullet(float x = 0, float y = 0) : position(Vector2{}),
                                       velocity(Vector2{}),
                                       width(5.f),
                                       height(13.f),
                                       color(Color{0.9f, 0.8f, 0.1f})
    {
        position.x = x;
        position.y = y;

        for (int i = 0; i < 10; i++)
        {
            Particle p{
                x,
                y};
            p.color.r = 1.f;
            p.color.g = 1.f;
            p.color.b = 1.f;
            srand(time(0) + i);
            float deg = (float(rand()) / 100.f);
            p.velocity.x = SDL_cosf(deg) * 0.1f;
            p.velocity.y = SDL_sinf(deg) * 0.1f;
            particles.push_back(p);
        }
    }

    void Draw(SDL_Renderer *renderer);
    void Update(uint32_t deltaTime);
};
#pragma once
#include <SDL3/SDL.h>
#include "vector2.h"
#include "images.h"

struct Alien
{
    Vector2 position{};
    float width = 0.f; 
    float height = 0.f;

    Alien(float x, float y, float w, float h): width(w), height(h) {
        position.x = x;
        position.y = y;
    } 

    void Draw(SDL_Renderer* renderer);
};
#pragma once
#include <SDL3/SDL.h>
//#include <vector>
#include "vector.h"
#include "bullet.h"

struct Player {
    Vector2 position{0, 0}; 
    float radius = 0.f;
    float speed = 0.5f;
    //std::vector<Bullet> bullets;

    Player(Vector2 pos = Vector2{}, float r = 10.f): position(Vector2{}), radius(r) {
        position.x = pos.x;
        position.y = pos.y;
    };

    void Draw(SDL_Renderer* renderer);
    void Move(int dir, uint32_t deltaTime);
};
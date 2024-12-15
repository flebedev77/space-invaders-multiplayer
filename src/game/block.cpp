#include "block.h"

void Block::Draw(SDL_Renderer* renderer)
{
    SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };

    Color blockColors[] = { Color{ 0.f, 1.f, 0.f }, Color{ 0.3f, 0.7f, 0.f}, Color{ 0.7f, 0.0f, 0.f} };
    int health = this->health;
    int blockHealthStatus = 0;

    if (health < 74) blockHealthStatus = 1;
    if (health < 49) blockHealthStatus = 2;
    if (health < 24) blockHealthStatus = 3;
    
    Color c = blockColors[blockHealthStatus];
    SDL_SetRenderDrawColorFloat(renderer, c.r, c.g, c.b, 0.f);

    SDL_RenderFillRect(renderer, &rect);
}
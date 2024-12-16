#include "particle.h"

void Particle::Draw(SDL_Renderer* renderer)
{
    SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };
    SDL_SetRenderDrawColorFloat(renderer, this->color.r, this->color.g, this->color.b, 1.f);
    SDL_RenderFillRect(renderer, &rect);
}

void Particle::Update(uint32_t deltaTime)
{
    this->position.x += this->velocity.x * deltaTime;
    this->position.y += this->velocity.y * deltaTime;
}
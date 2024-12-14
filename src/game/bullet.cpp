#include "bullet.h"

void Bullet::Draw(SDL_Renderer *renderer) {
    SDL_FRect rect = {
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };

    SDL_RenderFillRect(renderer, &rect);
}

void Bullet::Update(uint32_t deltaTime) {
}

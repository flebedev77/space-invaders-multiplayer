#include "bullet.h"

void Bullet::Draw(SDL_Renderer *renderer) {
    SDL_FRect rect = {
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };

    //SDL_RenderFillRect(renderer, &rect);
    SDL_RenderTexture(renderer, images::bullet_texture, &images::bullet_src_rect, &rect);
}

void Bullet::Update(uint32_t deltaTime) {
    this->position.x += this->velocity.x * deltaTime;
    this->position.y += this->velocity.y * deltaTime;
}

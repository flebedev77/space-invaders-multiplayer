#include "alien.h"

void Alien::Draw(SDL_Renderer* renderer)
{
    SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->width,
        this->height
    };

    SDL_RenderTexture(renderer, images::alien_texture, &images::alien_src_rect, &rect);
}
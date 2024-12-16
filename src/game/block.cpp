#include "block.h"

void Block::Draw(SDL_Renderer *renderer, uint32_t deltaTime)
{
    SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->width,
        this->height};

    Color blockColors[] = {Color{0.f, 1.f, 0.f}, Color{0.3f, 0.7f, 0.f}, Color{0.7f, 0.0f, 0.f}};
    int health = this->health;
    int blockHealthStatus = 0;

    SDL_Texture **tex = nullptr;

    if (health < 101)
    {
        tex = &images::zap1_texture;
        this->animated = true;
    }
    if (health < 74)
    {
        blockHealthStatus = 1;
        tex = &images::zap1_texture;
        this->animated = true;
    }
    if (health < 49)
    {
        blockHealthStatus = 2;
        tex = &images::zap2_texture;
        this->animated = false;
    }
    if (health < 24)
    {
        blockHealthStatus = 3;
        tex = &images::zap3_texture;
    }

    if (this->animated)
    {
        this->animationFrameDelay += deltaTime;
        if (this->animationFrameDelay > this->animationFrameRate)
        {
            this->animationFrameDelay = 0;
            this->frame = !this->frame;
        }
        tex = (this->frame) ? &images::zap1_texture : &images::zap1f2_texture;
    }

    Color c = blockColors[blockHealthStatus];
    SDL_SetRenderDrawColorFloat(renderer, c.r, c.g, c.b, 0.f);

    if (tex != nullptr)
    {
        SDL_RenderTexture(renderer, *(tex), &images::zap_src_rect, &rect);
    }
    else
    {
        SDL_RenderFillRect(renderer, &rect);
    }
}
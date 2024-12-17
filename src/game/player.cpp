#include "player.h"

void Player::Draw(SDL_Renderer *renderer, uint32_t deltaTime)
{
    // drawUtils::DrawCircle(renderer, this->position, this->radius);

    const SDL_FRect rect{
        this->position.x - this->radius,
        this->position.y - this->radius,
        this->radius * images::player_aspect,
        this->radius};

    SDL_Texture **tex;

    if (this->lookingDown)
    {
        tex = (this->selectedTexture) ? &images::player2_texture : &images::player2f2_texture;
    }
    else
    {
        tex = (this->selectedTexture) ? &images::player_texture : &images::playerf2_texture;
    }

    this->animationDelay += deltaTime;
    if (this->animationDelay > this->animationRate)
    {
        this->animationDelay = 0;
        this->selectedTexture = !this->selectedTexture;
    }

    if (this->blinking)
    {
        this->blinkingDelay += deltaTime;
        if (this->blinkingDelay > this->blinkingRate)
        {
            this->blinkingDelay = 0;
            this->blinkSpriteCurrent = !this->blinkSpriteCurrent;
        }

        if (this->blinkSpriteCurrent)
        {
            tex = (this->lookingDown) ? &images::images[1].texture : &images::images[0].texture;
        }
    }

    if (tex != nullptr && this->health > 0)
    {
        SDL_RenderTexture(renderer, *(tex), &images::player_src_rect, &rect);
    }
}

void Player::Move(int direction, uint32_t deltaTime)
{
    this->position.x += this->speed * float(direction) * deltaTime;
}

void Player::Shoot()
{
    this->prevShotLeft = !this->prevShotLeft;
    float num = (this->prevShotLeft) ? 15.f : -15.f;
    Bullet b{
        this->position.x - 2.5f + num,
        this->position.y - 25.f};

    // Bullet b2{
    //     this->position.x - 2.5f + 15.f,
    //     this->position.y - 25.f};

    b.velocity.y = (this->lookingDown) ? 0.3f : -0.3f;
    // b2.velocity.y = -0.3f;
    this->bullets.push_back(b);
    // this->bullets.push_back(b2);
}
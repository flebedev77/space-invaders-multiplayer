#include "player.h"

bool prevShotLeft = false;

void Player::Draw(SDL_Renderer *renderer)
{
    // drawUtils::DrawCircle(renderer, this->position, this->radius);

    const SDL_FRect rect{
        this->position.x - this->radius,
        this->position.y - this->radius,
        this->radius * images::player_aspect,
        this->radius};

    SDL_RenderTexture(renderer, images::player_texture, &images::player_src_rect, &rect);
}

void Player::Move(int direction, uint32_t deltaTime)
{
    this->position.x += this->speed * float(direction) * deltaTime;
}

void Player::Shoot()
{
    prevShotLeft = !prevShotLeft;
    float num = (prevShotLeft) ? 15.f : -15.f;
    Bullet b{
        this->position.x - 2.5f + num,
        this->position.y - 25.f};

    // Bullet b2{
    //     this->position.x - 2.5f + 15.f,
    //     this->position.y - 25.f};

    b.velocity.y = -0.3f;
    //b2.velocity.y = -0.3f;
    this->bullets.push_back(b);
    //this->bullets.push_back(b2);
}
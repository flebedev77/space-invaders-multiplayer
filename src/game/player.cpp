#include "player.h"

void Player::Draw(SDL_Renderer *renderer)
{
    //drawUtils::DrawCircle(renderer, this->position, this->radius);
    const SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->radius,
        this->radius
    };

    SDL_RenderTexture(renderer, images::player_texture, images::player_src_rect, &rect);
}

void Player::Move(int direction, uint32_t deltaTime)
{
    this->position.x += this->speed * float(direction) * deltaTime;
}

void Player::Shoot()
{
    Bullet b{
        this->position.x,
        this->position.y
    };
    b.velocity.y = -0.3f;
    this->bullets.push_back(b);
}
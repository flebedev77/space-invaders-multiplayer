#include "player.h"

void Player::Draw(SDL_Renderer *renderer)
{
    drawUtils::DrawCircle(renderer, this->position, this->radius);
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
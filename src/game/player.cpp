#include "player.h"
#include "drawutils.h"

void Player::Draw(SDL_Renderer* renderer) {
    drawUtils::DrawCircle(renderer, this->position, this->radius);
}

void Player::Move(int direction, uint32_t deltaTime) {
    this->position.x += this->speed * float(direction) * deltaTime;
}
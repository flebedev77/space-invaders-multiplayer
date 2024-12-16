#include "alien.h"

void Alien::Draw(SDL_Renderer *renderer)
{
    SDL_FRect rect{
        this->position.x,
        this->position.y,
        this->width,
        this->height};

    SDL_RenderTexture(renderer, images::alien_texture, &images::alien_src_rect, &rect);
}

void Alien::Update_Shoot(SDL_Renderer *renderer, uint32_t deltaTime, std::vector<Block> &blocks, Player &player)
{
    this->shootDelay += deltaTime;
    if (this->shootDelay > this->shootRate)
    {
        this->shootDelay = 0;

        Bullet b{
            this->position.x,
            this->position.y};

        b.velocity.y = 0.3f;

        this->bullets.push_back(b);
    }

    for (size_t i = 0; i < this->bullets.size(); i++)
    {
        Bullet &bullet = this->bullets.at(i);

        bullet.Update(deltaTime);
        bullet.Draw(renderer);

        bool willDie = false;

        SDL_FRect bullet_rect{
            bullet.position.x,
            bullet.position.y,
            bullet.width,
            bullet.height};

        for (size_t blockIndex = 0; blockIndex < blocks.size(); blockIndex++)
        {
            Block &block = blocks.at(blockIndex);

            SDL_FRect block_rect{
                block.position.x,
                block.position.y,
                block.width,
                block.height};

            if (utils::aabb(&block_rect, &bullet_rect))
            {
                block.health -= this->bulletDamage;
                willDie = true;

                if (block.health <= 0)
                {
                    blocks.erase(blocks.begin() + blockIndex);
                }
                break;
            }
        }

        if (!willDie)
        {
            SDL_FRect player_rect{
                player.position.x - player.radius,
                player.position.y - player.radius,
                player.radius * images::player_aspect,
                player.radius
            };

            if (utils::aabb(&bullet_rect, &player_rect))
            {
                willDie = true;

                player.health -= this->bulletDamage;
            }
        }

        if (willDie)
        {
            this->bullets.erase(this->bullets.begin() + i);
        }
    }
}
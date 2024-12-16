#include "alienmanager.h"

namespace alienmgr
{
    namespace config
    {
        constexpr float alienWidth = 15.f;
        constexpr float alienHeight = 15.f;
        constexpr float alienPadding = 1.4f;
        constexpr float alienSpeed = 0.1f;
        constexpr float moveDownAmount = 3.f;
    };

    void MoveDown(std::vector<Alien> &aliens, float moveDownAmount);

    int alienDirection = 1;

    void SpawnAliens(int amt_h, int amt_v, std::vector<Alien> &aliens)
    {
        for (int x = 0; x < amt_h; x++)
        {
            for (int y = 0; y < amt_v; y++)
            {
                Alien a{
                    x * config::alienWidth * images::alien_aspect * config::alienPadding,
                    y * config::alienHeight * config::alienPadding,
                    config::alienWidth * images::alien_aspect, config::alienHeight};
                srand(time(0) + ((x + y) * 1000));
                a.shootDelay = rand() % (a.shootRate + 1);
                aliens.push_back(a);
            }
        }
    }

    void UpdateAliens(SDL_Renderer *renderer, std::vector<Alien> &aliens, std::vector<Block> &blocks, int windowWidth, int windowHeight, uint32_t deltaTime, Player &player, float moveDownAmount, std::vector<Particle> &particles)
    {
        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);

            alien.Draw(renderer, deltaTime);
            alien.Update_Shoot(renderer, deltaTime, blocks, player, windowHeight);

            alien.position.x += float(alienDirection) * config::alienSpeed * deltaTime;

            for (size_t alienBulletIndex = 0; alienBulletIndex < alien.bullets.size(); alienBulletIndex++)
            {
                Bullet &alienBullet = alien.bullets.at(alienBulletIndex);
                SDL_FRect alienBulletRect{
                    alienBullet.position.x,
                    alienBullet.position.y,
                    alienBullet.width,
                    alienBullet.height};

                for (size_t playerBulletIndex = 0; playerBulletIndex < player.bullets.size(); playerBulletIndex++)
                {
                    Bullet &playerBullet = player.bullets.at(playerBulletIndex);
                    SDL_FRect playerBulletRect{
                        playerBullet.position.x,
                        playerBullet.position.y,
                        playerBullet.width,
                        playerBullet.height};

                    if (utils::aabb(&alienBulletRect, &playerBulletRect))
                    {
                        alien.bullets.erase(alien.bullets.begin() + alienBulletIndex);
                        player.bullets.erase(player.bullets.begin() + playerBulletIndex);

                        for (int j = 0; j < 5; j++)
                        {
                            Particle p{
                                alienBullet.position.x,
                                alienBullet.position.y};
                            p.color.r = 0.5f;
                            p.color.g = 0.f;
                            p.color.b = 0.5f;
                            srand(time(0) + i);
                            float deg = (float(rand()) / 1000.f);
                            p.velocity.x = SDL_cosf(deg) * 0.1f;
                            p.velocity.y = SDL_sinf(deg) * 0.1f;
                            particles.push_back(p);
                        }

                        for (int j = 0; j < 5; j++)
                        {
                            Particle p{
                                playerBullet.position.x,
                                playerBullet.position.y};
                            p.color.r = 0.9f;
                            p.color.g = 0.8f;
                            p.color.b = 0.1f;
                            srand(time(0) + j);
                            float deg = (float(rand()) / 1000.f);
                            p.velocity.x = SDL_cosf(deg) * 0.1f;
                            p.velocity.y = SDL_sinf(deg) * 0.1f;
                            particles.push_back(p);
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);
            if (alien.position.x + alien.width > float(windowWidth))
            {
                alienDirection = -1;
                MoveDown(aliens, moveDownAmount);
                break;
            }
            else if (alien.position.x < 0.f)
            {
                alienDirection = 1;
                MoveDown(aliens, moveDownAmount);
                break;
            }
        }
    }

    void MoveDown(std::vector<Alien> &aliens, float moveDownAmount)
    {
        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);

            alien.position.y += moveDownAmount;
        }
    }
};
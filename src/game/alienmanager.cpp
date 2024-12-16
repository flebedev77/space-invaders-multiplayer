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

    void MoveDown(std::vector<Alien> &aliens);

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

                aliens.push_back(a);
            }
        }
    }

    void UpdateAliens(SDL_Renderer *renderer, std::vector<Alien> &aliens, int windowWidth, uint32_t deltaTime)
    {
        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);

            alien.Draw(renderer);

            alien.position.x += float(alienDirection) * config::alienSpeed * deltaTime;
        }

        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);
            if (alien.position.x + alien.width > float(windowWidth))
            {
                alienDirection = -1;
                MoveDown(aliens);
                break;
            }
            else if (alien.position.x < 0.f)
            {
                alienDirection = 1;
                MoveDown(aliens);
                break;
            }
        }
    }

    void MoveDown(std::vector<Alien> &aliens)
    {
        for (size_t i = 0; i < aliens.size(); i++)
        {
            Alien &alien = aliens.at(i);

            alien.position.y += config::moveDownAmount;
        }
    }
};
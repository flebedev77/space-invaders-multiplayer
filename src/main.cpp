#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>
#include "game/images.h"
#include "game/player.h"
#include "game/block.h"
#include "game/alienmanager.h"
#include "game/alien.h"
#include "game/util.h"
#include "game/config.h"

struct AppContext
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
};

void SpawnParticles(float x, float y, int amt, float r, float g, float b);

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // init the library, here we make a window so we only need the Video capabilities.
    if (not SDL_Init(SDL_INIT_VIDEO))
    {
        return SDL_Fail();
    }

    // create a window
    SDL_Window *window = SDL_CreateWindow("SPACE INVADERS", config::windowWidth, config::windowHeight, SDL_WINDOW_RESIZABLE);
    if (not window)
    {
        return SDL_Fail();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (not renderer)
    {
        return SDL_Fail();
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth)
        {
            SDL_Log("This is a highdpi environment.");
        }
    }

    // set up the application data
    *appstate = new AppContext{
        window,
        renderer,
    };

    SDL_Log("Application started successfully!");

    config::player.position.y = (float)config::windowHeight - 20;
    config::player.position.x = (float)config::windowWidth / 2;

    int amt_horizontal = config::windowWidth / config::blockWidth;
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < amt_horizontal; i++)
        {
            srand(time(0) + i * 10);
            Block b{
                float(i) * config::blockWidth,
                float(config::windowHeight - config::wallHeightFromFloor - (j * config::blockHeight)),
                config::blockWidth, config::blockHeight,
                config::blockMaxHealth, static_cast<uint32_t>(rand())};
            config::blocks.push_back(b);
        }
    }

    images::LoadImages(renderer);

    alienmgr::SpawnAliens(config::alien_amt_h, config::alien_amt_v, config::aliens);

    for (size_t i = 0; i < config::stars_amt; i++)
    {
        srand(time(0) + i * 100);
        float starx = float(rand() % config::windowWidth);
        srand(time(0) + i * 120);
        float stary = float(rand() % config::windowHeight);

        config::stars[i].position.x = starx;
        config::stars[i].position.y = stary;
        config::stars[i].color.r = 1.f;
        config::stars[i].color.g = 1.f;
        config::stars[i].color.b = 1.f;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    auto *app = (AppContext *)appstate;

    if (
        event->type == SDL_EVENT_QUIT ||
        (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_END))
    {
        images::UnloadImages();
        app->app_quit = SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP)
    {
        bool down = (event->type == SDL_EVENT_KEY_DOWN);
        if (event->key.key == SDLK_A || event->key.key == SDLK_LEFT)
        {
            config::keys::left = down;
        }
        if (event->key.key == SDLK_D || event->key.key == SDLK_RIGHT)
        {
            config::keys::right = down;
        }
        if (event->key.key == SDLK_SPACE)
        {
            config::keys::shoot = down;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    uint32_t currentTicks = SDL_GetTicks();
    config::deltaTime = currentTicks - config::prevTime;
    config::prevTime = currentTicks;

    auto *app = (AppContext *)appstate;
    SDL_Renderer *renderer = (*app).renderer;

    SDL_SetRenderDrawColor(renderer, config::backgroundColor.r, config::backgroundColor.g, config::backgroundColor.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    for (size_t i = 0; i < config::stars_amt; i++)
    {
        Particle &p = config::stars[i];
        p.Draw(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 255, 4, 4, 255);
    SDL_RenderDebugText(renderer, config::windowWidth / 2 - 50, 10, "SPACE INVADERS");
    config::player.Draw(renderer, config::deltaTime);

    if (config::keys::left)
    {
        config::player.Move(-1, config::deltaTime);
    }
    if (config::keys::right)
    {
        config::player.Move(1, config::deltaTime);
    }

    config::shootDelay += config::deltaTime;
    if (config::keys::shoot && config::shootDelay > config::shootRate)
    {
        config::shootDelay = 0;
        config::player.Shoot();
    }

    if (config::player.position.x < config::player.radius)
    {
        config::player.position.x = config::player.radius;
    }
    if (config::player.position.x > config::windowWidth - config::player.radius)
    {
        config::player.position.x = config::windowWidth - config::player.radius;
    }

    for (size_t i = 0; i < config::global_particles.size(); i++)
    {
        Particle &particle = config::global_particles.at(i);
        particle.Draw(renderer);
        particle.Update(config::deltaTime);

        particle.lifetime += config::deltaTime;
        if (particle.lifetime > particle.maxLifetime)
        {
            config::global_particles.erase(config::global_particles.begin() + i);
        }
    }

    for (size_t i = 0; i < config::player.bullets.size(); i++)
    {
        Bullet &bullet = config::player.bullets.at(i);

        bullet.Update(config::deltaTime);
        bullet.Draw(renderer);

        bool willDie = (bullet.position.y < -bullet.height);

        if (!willDie)
        {
            SDL_FRect bulletRect{
                bullet.position.x,
                bullet.position.y,
                bullet.width,
                bullet.height};
            for (size_t blockIndex = 0; blockIndex < config::blocks.size(); blockIndex++)
            {
                Block &block = config::blocks.at(blockIndex);
                SDL_FRect blockRect{
                    block.position.x,
                    block.position.y,
                    block.width,
                    block.height};

                if (utils::aabb(&blockRect, &bulletRect))
                {
                    willDie = true;
                    block.health -= config::bulletDamage;
                    SpawnParticles(block.position.x, block.position.y, 5, 0.f, 0.f, 1.f);
                    if (block.health <= 0)
                    {
                        config::blocks.erase(config::blocks.begin() + blockIndex);
                    }
                    break;
                }
            }

            for (size_t alienIndex = 0; alienIndex < config::aliens.size(); alienIndex++)
            {
                Alien &alien = config::aliens.at(alienIndex);

                SDL_FRect alienRect{
                    alien.position.x,
                    alien.position.y,
                    alien.width,
                    alien.height};

                if (utils::aabb(&bulletRect, &alienRect))
                {
                    willDie = true;
                    alien.health -= config::bulletDamage;
                    SpawnParticles(alien.position.x, alien.position.y, 10, 0.5f, 0.f, 0.5f);
                    if (alien.health <= 0)
                    {
                        SpawnParticles(alien.position.x, alien.position.y, 50, 1.f, 0.f, 1.f);
                        config::aliens.erase(config::aliens.begin() + alienIndex);
                    }
                    break;
                }
            }
        }

        if (willDie)
        {
            config::player.bullets.erase(config::player.bullets.begin() + i);
        }
    }

    for (size_t i = 0; i < config::blocks.size(); i++)
    {
        Block &block = config::blocks.at(i);

        block.Draw(renderer, config::deltaTime);
    }

    alienmgr::UpdateAliens(renderer, config::aliens, config::blocks, config::windowWidth, config::deltaTime, config::player, config::alienMoveDownAmount);

    if (config::aliens.size() == 0)
    {
        config::isSwitchingNextStage = true;
        if (config::alien_amt_h < 10)
        {
            config::alien_amt_h++;
        }
        config::alien_amt_v++;
        config::stageNum++;
        alienmgr::SpawnAliens(config::alien_amt_h, config::alien_amt_v, config::aliens);
    }

    if (config::isSwitchingNextStage)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDebugText(renderer, config::windowWidth/2 - 10, config::windowHeight/2, ("Stage " + std::to_string(config::stageNum)).c_str());

        config::switchingNextStageDelay += config::deltaTime;
        if (config::switchingNextStageDelay > config::switchingNextStageRate)
        {
            config::switchingNextStageDelay = 0;
            config::isSwitchingNextStage = false;
        }
    }

    config::isGameOver = (config::aliens.back().position.y > float(config::windowHeight - config::wallHeightFromFloor - 5.f)) || (config::player.health <= 0);

    // health bar
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_FRect healthBarRect{
        0,
        config::windowHeight - config::healthbarHeight,
        config::healthbarWidth,
        config::healthbarHeight};
    SDL_RenderFillRect(renderer, &healthBarRect);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    config::player.playerSmoothHealth = utils::lerp(config::player.playerSmoothHealth, float(config::player.health), 0.01f * float(config::deltaTime));
    float realHealthValue = (float(config::player.playerSmoothHealth) / float(config::player.maxHealth)) * config::healthbarWidth;
    healthBarRect.w = realHealthValue;
    SDL_RenderFillRect(renderer, &healthBarRect);

    if (config::isGameOver)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDebugText(renderer, float(config::windowWidth / 2) - 50.f, float(config::windowHeight / 2), "GAME OVER!");
    }

    SDL_RenderPresent(app->renderer);

    return app->app_quit;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    auto *app = (AppContext *)appstate;
    if (app)
    {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    SDL_Log("Application quit successfully!");
}

void SpawnParticles(float x, float y, int amt, float r, float g, float b)
{
    for (int i = 0; i < amt; i++)
    {
        Particle p{
            x,
            y};
        p.color.r = r;
        p.color.g = g;
        p.color.b = b;
        srand(time(0) + i);
        float deg = (float(rand()) / 1000.f);
        p.velocity.x = SDL_cosf(deg) * 0.1f;
        p.velocity.y = SDL_sinf(deg) * 0.1f;
        config::global_particles.push_back(p);
    }
}

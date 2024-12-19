#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>
#include <string>
#include <cstring>
#include "game/images.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/block.h"
#include "game/util.h"
#include "game/config.h"
#include "game/controls.h"
#include "logo.h"

struct AppContext
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
};


void SpawnParticles(float x, float y, int amt, float r, float g, float b, float spd = 0.1f);

void invaders_init()
{

    for (size_t i = 0; i < config::playerAmt; i++)
    {
        config::players[i].health = config::players[i].maxHealth;
        config::players[i].position.y = config::playerSpawnPositions[i].y;
        config::players[i].position.x = config::playerSpawnPositions[i].x;
        config::players[i].blinking = false;
    }
    config::players[0].lookingDown = 1;

    config::blocks.clear();
    int amt_horizontal = config::windowWidth / config::blockWidth;
    for (int j = 0; j < 1; j++)
    {
        // top
        for (int i = 0; i < amt_horizontal; i++)
        {
            if (i % 2 == 1 || i % 3 == 1)
                continue;
            srand(time(0) + i * 10);
            Block b{
                float(i) * config::blockWidth,
                float(config::wallHeightFromFloor + (j * config::blockHeight) - config::blockHeight),
                config::blockWidth, config::blockHeight,
                config::blockMaxHealth, static_cast<uint32_t>(rand())};
            b.animationFrameDelay = i * 10;
            config::blocks.push_back(b);
        }

        // bottom
        for (int i = 0; i < amt_horizontal; i++)
        {
            if (i % 2 == 0 || i % 3 == 0)
                continue;
            srand(time(0) + i * 10);
            Block b{
                float(i) * config::blockWidth,
                float(config::windowHeight - config::wallHeightFromFloor - (j * config::blockHeight)),
                config::blockWidth, config::blockHeight,
                config::blockMaxHealth, static_cast<uint32_t>(rand())};
            b.animationFrameDelay = i * 10;
            config::blocks.push_back(b);
        }
    }
}

SDL_AppResult SDL_Fail()
{
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    // init the library, here we make a window so we only need the Video capabilities.
    if (not SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
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

    audio::LoadAudios();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    SDL_SetWindowIcon(window, utils::loadEmbeddedBMP(renderer, logo_bmp, logo_bmp_len));

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
        renderer};

    SDL_Log("Application started successfully!");

    invaders_init();

    images::LoadImages(renderer);

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
    SDL_SetRenderViewport(renderer, &config::cameraPos);

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
        if (down)
        {
            if (!config::hasStarted)
            {
                config::hasStarted = true;
            }
            else if (config::isGameOver && config::gameOverRestartDelay > config::gameOverRestartRate)
            {
                config::gameOverRestartDelay = 0;
                invaders_init();
            }
        }

        if (event->key.key == SDLK_A)
        {
            config::keys[0].left = down;
        }
        if (event->key.key == SDLK_D)
        {
            config::keys[0].right = down;
        }
        if (event->key.key == SDLK_SPACE)
        {
            config::keys[0].shoot = down;
        }

        if (event->key.key == SDLK_LEFT)
        {
            config::keys[1].left = down;
        }
        if (event->key.key == SDLK_RIGHT)
        {
            config::keys[1].right = down;
        }
        if (event->key.key == SDLK_RCTRL)
        {
            config::keys[1].shoot = down;
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

    config::frameIndex++;

    utils::cameraUpdate(renderer, config::smoothCameraPos, config::cameraPos, config::deltaTime, config::frameIndex, config::cameraSmoothness);
    SDL_SetRenderViewport(renderer, &config::smoothCameraPos);

    for (size_t i = 0; i < config::stars_amt; i++)
    {
        Particle &p = config::stars[i];
        p.Draw(renderer);
    }

    SDL_SetRenderDrawColor(renderer, 255, 4, 4, 255);
    SDL_RenderDebugText(renderer, config::windowWidth / 2 - 50, 10, "SPACE INVADERS");

    // player drawing, controls, collision
    for (size_t i = 0; i < config::playerAmt; i++)
    {
        config::players[i].Draw(renderer, config::deltaTime);
        if (config::keys[i].left)
        {
            config::players[i].Move(-1, config::deltaTime);
        }
        if (config::keys[i].right)
        {
            config::players[i].Move(1, config::deltaTime);
        }

        config::players[i].shootDelay += config::deltaTime;
        if (config::keys[i].shoot && config::players[i].shootDelay > config::shootRate)
        {
            config::players[i].shootDelay = 0;
            config::players[i].Shoot();
            audio::PlaySound(audio::sounds[audio::Sound::shootSound]);
        }

        Player &player = config::players[i];
        if (player.position.x < player.radius)
        {
            player.position.x = player.radius;
        }
        if (player.position.x > config::windowWidth - player.radius)
        {
            player.position.x = config::windowWidth - player.radius;
        }
    }

    // global particles
    if (config::global_particles.size() > 0)
    {
        for (auto it = config::global_particles.rbegin(); it != config::global_particles.rend(); ++it)
        {
            Particle &particle = *it;
            particle.Draw(renderer);
            particle.Update(config::deltaTime);

            particle.lifetime += config::deltaTime;
            if (particle.lifetime > particle.maxLifetime)
            {
                // config::global_particles.erase(config::global_particles.begin() + i);
                config::global_particles.erase((it + 1).base());
            }
        }
    }

    // player bullet update and block collision
    for (size_t playerIndex = 0; playerIndex < config::playerAmt; playerIndex++)
    {
        Player &player = config::players[playerIndex];
        for (auto bulletIter = player.bullets.rbegin(); bulletIter != player.bullets.rend(); ++bulletIter)
        {
            Bullet &bullet = *bulletIter;

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
                for (auto blockIter = config::blocks.rbegin(); blockIter != config::blocks.rend(); ++blockIter)
                {
                    Block &block = *blockIter;
                    SDL_FRect blockRect{
                        block.position.x,
                        block.position.y,
                        block.width,
                        block.height};

                    if (utils::aabb(&blockRect, &bulletRect))
                    {
                        willDie = true;
                        block.health -= config::bulletDamage;
                        audio::PlaySound(audio::sounds[audio::Sound::zapSound]);
                        SpawnParticles(block.position.x, block.position.y, 5, 0.5f, 0.5f, 1.f);
                        if (block.health <= 0)
                        {
                            config::blocks.erase((blockIter + 1).base());
                        }
                        break;
                    }
                }
            }

            if (willDie)
            {
                player.bullets.erase((bulletIter + 1).base());
            }
        }
    }

    // blocks
    for (size_t i = 0; i < config::blocks.size(); i++)
    {
        Block &block = config::blocks.at(i);

        block.Draw(renderer, config::deltaTime);
    }

    // player bullet to player collision
    for (size_t i = 0; i < config::playerAmt - 1; i++)
    {
        Player &a = config::players[i];
        SDL_FRect a_rect{
            a.position.x - a.radius,
            a.position.y - a.radius,
            a.radius * images::player_aspect,
            a.radius};

        for (size_t j = i + 1; j < config::playerAmt; j++)
        {
            Player &b = config::players[j];
            SDL_FRect b_rect{
                b.position.x - b.radius,
                b.position.y - b.radius,
                b.radius * images::player_aspect,
                b.radius};

            for (size_t bulletIndex = 0; bulletIndex < a.bullets.size(); bulletIndex++)
            {
                Bullet &bullet = a.bullets.at(bulletIndex);
                SDL_FRect bullet_rect{
                    bullet.position.x,
                    bullet.position.y,
                    bullet.width,
                    bullet.height};

                if (utils::aabb(&b_rect, &bullet_rect))
                {
                    b.health -= config::bulletDamage;
                    audio::PlaySound(audio::sounds[audio::Sound::hitSound]);
                    SpawnParticles(b.position.x, b.position.y, 25, 1.f, 1.f, 1.f, 0.5f);
                    utils::cameraShake(config::cameraPos);
                    a.bullets.erase(a.bullets.begin() + bulletIndex);
                    b.hit = true;
                }
            }

            for (auto bBulletIter = b.bullets.rbegin(); bBulletIter != b.bullets.rend(); ++bBulletIter)
            {
                Bullet &bullet = *bBulletIter;
                SDL_FRect bullet_rect{
                    bullet.position.x,
                    bullet.position.y,
                    bullet.width,
                    bullet.height};

                if (utils::aabb(&a_rect, &bullet_rect))
                {
                    a.health -= config::bulletDamage;
                    audio::PlaySound(audio::sounds[audio::Sound::hitSound]);
                    a.hit = true;
                    SpawnParticles(a.position.x, a.position.y, 25, 1.f, 1.f, 1.f, 0.5f);
                    utils::cameraShake(config::cameraPos);

                    b.bullets.erase((bBulletIter + 1).base());
                }
                for (auto aBulletIter = a.bullets.rbegin(); aBulletIter != a.bullets.rend(); ++aBulletIter)
                {
                    Bullet &aBullet = *aBulletIter;

                    SDL_FRect a_bullet_rect{
                        aBullet.position.x,
                        aBullet.position.y,
                        aBullet.width,
                        aBullet.height};

                    if (utils::aabb(&bullet_rect, &a_bullet_rect))
                    {
                        SpawnParticles(bullet.position.x, bullet.position.y, 15, 0.9f, 0.8f, 0.1f);
                        utils::cameraShake(config::cameraPos);
                        audio::PlaySound(audio::sounds[audio::Sound::thudSound]);
                        b.bullets.erase((bBulletIter + 1).base());
                        a.bullets.erase((aBulletIter + 1).base());
                    }
                }
            }
        }
    }

    config::isGameOver = (config::players[0].health <= 0 || config::players[1].health <= 0);

    if (config::hasStarted)
    {
        // health bar
        for (size_t i = 0; i < config::playerAmt; i++)
        {
            Player &player = config::players[i];
            float y = (i == 0) ? 0 : config::windowHeight - config::healthbarHeight;
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_FRect healthBarRect{
                0,
                y,
                config::healthbarWidth,
                config::healthbarHeight};
            SDL_RenderFillRect(renderer, &healthBarRect);
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            player.playerSmoothHealth = utils::lerp(player.playerSmoothHealth, float(player.health), 0.01f * float(config::deltaTime));
            float realHealthValue = (float(player.playerSmoothHealth) / float(player.maxHealth)) * config::healthbarWidth;
            healthBarRect.w = realHealthValue;
            SDL_RenderFillRect(renderer, &healthBarRect);
        }
    }
    if (config::hasStarted == false || config::isGameOver)
    {
        config::menuScreenColorChangeDelay += config::deltaTime;
        if (config::menuScreenColorChangeDelay > config::menuScreenColorChangeRate)
        {
            config::menuScreenColorChangeDelay = 0;
            config::menuScreenColorIndex = (config::menuScreenColorIndex + 1) % 5;
        }
        Color textColors[] = {Color{1.f, 0.f, 0.f}, Color{0.f, 1.f, 0.f}, Color{0.f, 0.f, 1.f}, Color{0.5f, 0.5f, 0.f}, Color{0.9f, 0.8f, 0.1f}};
        Color c = textColors[config::menuScreenColorIndex];

        SDL_SetRenderDrawColorFloat(renderer, c.r, c.b, c.g, 1.f);
        if (config::hasStarted == false)
        {
            SDL_RenderDebugText(renderer, config::windowWidth / 2 - 100, config::windowHeight / 2, "PRESS ANY KEY TO START!!!");
        }
        else if (config::isGameOver)
        {
            config::gameOverRestartDelay += config::deltaTime;

            config::gameOverPlayerIndex = (config::players[0].health <= 0) ? 1 : 0;
            config::players[config::gameOverPlayerIndex].blinking = true;

            if (config::gameOverRestartDelay > config::gameOverRestartRate)
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                int index2 = (config::menuScreenColorIndex + 1) % 5;
                SDL_SetRenderDrawColorFloat(renderer, c.r, c.b, c.g, 1.f);
                SDL_RenderDebugText(renderer, float(config::windowWidth / 2) - 50.f, float(config::windowHeight / 2), ("Player " + std::to_string(config::gameOverPlayerIndex + 1) + " Won!!").c_str());
                c = textColors[index2];
                SDL_SetRenderDrawColorFloat(renderer, c.r, c.b, c.g, 1.f);
                SDL_RenderDebugText(renderer, float(config::windowWidth / 2) - 120.f, float(config::windowHeight / 2) + 50.f, "PRESS ANY KEY TO PLAY AGAIN!!!");
            }
        }
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

void SpawnParticles(float x, float y, int amt, float r, float g, float b, float spd)
{
    for (int i = 0; i < amt; i++)
    {
        Particle p{
            x,
            y};
        p.color.r = r;
        p.color.g = g;
        p.color.b = b;
        // srand(time(0) + i);
        float deg = float(i); //(float(rand()) / 1000.f);
        p.velocity.x = SDL_cosf(deg) * spd;
        p.velocity.y = SDL_sinf(deg) * spd;
        config::global_particles.push_back(p);
    }
}

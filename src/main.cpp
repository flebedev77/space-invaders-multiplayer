#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <ctime>
#include <random>
#include <cstdlib>
#include <string>
#include <cstring>
#include "game/images.h"
#include "game/player.h"
#include "game/block.h"
#include "game/util.h"
#include "game/config.h"
#include "game/controls.h"

struct AppContext
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_AppResult app_quit = SDL_APP_CONTINUE;
};

void SpawnParticles(float x, float y, int amt, float r, float g, float b);

void invaders_init()
{

	for (size_t i = 0; i < config::playerAmt; i++)
	{
		config::players[i].health = config::players[i].maxHealth;
		config::players[i].position.y = config::playerSpawnPositions[i].y;
		config::players[i].position.x = config::playerSpawnPositions[i].x;
	}

	config::blocks.clear();
	int amt_horizontal = config::windowWidth / config::blockWidth;
	for (int j = 0; j < 2; j++)
	{
		// top
		for (int i = 0; i < amt_horizontal; i++)
		{
			srand(time(0) + i * 10);
			Block b{
				float(i) * config::blockWidth,
				float(config::wallHeightFromFloor + (j * config::blockHeight) - config::blockHeight),
				config::blockWidth, config::blockHeight,
				config::blockMaxHealth, static_cast<uint32_t>(rand())};
			config::blocks.push_back(b);
		}

		// bottom
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
}

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
		if (!config::hasStarted)
		{
			config::hasStarted = true;
		}
		else if (config::isGameOver)
		{
			invaders_init();
		}

		bool down = (event->type == SDL_EVENT_KEY_DOWN);
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

	for (size_t i = 0; i < config::stars_amt; i++)
	{
		Particle &p = config::stars[i];
		p.Draw(renderer);
	}

	SDL_SetRenderDrawColor(renderer, 255, 4, 4, 255);
	SDL_RenderDebugText(renderer, config::windowWidth / 2 - 50, 10, "SPACE INVADERS");
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
		}
	}

	for (size_t i = 0; i < config::playerAmt; i++)
	{
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

	for (size_t playerIndex = 0; playerIndex < config::playerAmt; playerIndex++)
	{
		Player &player = config::players[playerIndex];
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			Bullet &bullet = player.bullets.at(i);

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
			}

			if (willDie)
			{
				player.bullets.erase(player.bullets.begin() + i);
			}
		}
	}

	for (size_t i = 0; i < config::blocks.size(); i++)
	{
		Block &block = config::blocks.at(i);

		block.Draw(renderer, config::deltaTime);
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
		else
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
			int index2 = (config::menuScreenColorIndex + 1) % 5;
			SDL_SetRenderDrawColorFloat(renderer, c.r, c.b, c.g, 1.f);
			SDL_RenderDebugText(renderer, float(config::windowWidth / 2) - 45.f, float(config::windowHeight / 2), "GAME OVER!");
			c = textColors[index2];
			SDL_SetRenderDrawColorFloat(renderer, c.r, c.b, c.g, 1.f);
			SDL_RenderDebugText(renderer, float(config::windowWidth / 2) - 120.f, float(config::windowHeight / 2) + 50.f, "PRESS ANY KEY TO PLAY AGAIN!!!");
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

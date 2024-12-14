#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <iostream>
#include "game/config.h"
#include "game/player.h"
#include "game/drawutils.h"

struct AppContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_AppResult app_quit = SDL_APP_CONTINUE;
};

SDL_AppResult SDL_Fail(){
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return SDL_APP_FAILURE;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // init the library, here we make a window so we only need the Video capabilities.
    if (not SDL_Init(SDL_INIT_VIDEO)){
        return SDL_Fail();
    }
    
    // create a window
    SDL_Window* window = SDL_CreateWindow("Space Invaders", config::windowWidth, config::windowHeight, SDL_WINDOW_BORDERLESS);
    if (not window){
        return SDL_Fail();
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (not renderer){
        return SDL_Fail();
    }
    
    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth){
            SDL_Log("This is a highdpi environment.");
        }
    }

    // set up the application data
    *appstate = new AppContext{
       window,
       renderer,
    };
    
    SDL_Log("Application started successfully!");
    SDL_Log("Initialising...");

    config::player.position.y = (float)config::windowHeight - 20;
    config::player.position.x = (float)config::windowWidth / 2;

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event* event) {
    auto* app = (AppContext*)appstate;
    
    if (
        event->type == SDL_EVENT_QUIT ||
        (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_END)
    ) {
        app->app_quit = SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN || event->type == SDL_EVENT_KEY_UP) {
        bool down = (event->type == SDL_EVENT_KEY_DOWN);
        if (event->key.key == SDLK_A || event->key.key == SDLK_LEFT) {
            config::keys::left = down;
        }
        if (event->key.key == SDLK_D || event->key.key == SDLK_RIGHT) {
            config::keys::right = down;
        }
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    uint32_t currentTicks = SDL_GetTicks();
    config::deltaTime = currentTicks - config::prevTime;
    config::prevTime = currentTicks;

    auto* app = (AppContext*)appstate;

    SDL_SetRenderDrawColor(app->renderer, config::backgroundColor.r, config::backgroundColor.g, config::backgroundColor.b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(app->renderer);

    SDL_SetRenderDrawColor(app->renderer, 255, 4, 4, 255);
    SDL_RenderDebugText(app->renderer, config::windowWidth/2 - 50, 10, "SPACE INVADERS");

    config::player.Draw(app->renderer);

    if (config::keys::left) {
        config::player.Move(-1, config::deltaTime);
    }
    if (config::keys::right) {
        config::player.Move(1, config::deltaTime);
    }

    SDL_RenderPresent(app->renderer);

    return app->app_quit;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    auto* app = (AppContext*)appstate;
    if (app) {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        delete app;
    }

    SDL_Log("Application quit successfully!");
}

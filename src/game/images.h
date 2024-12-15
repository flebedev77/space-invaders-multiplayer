#pragma once
#include <SDL3/SDL.h>
#include "util.h"

namespace images
{
    extern SDL_Surface *alien_surface;
    extern SDL_Surface *bullet_surface;
    extern SDL_Surface *player_surface;

    extern SDL_FRect *alien_src_rect;
    extern SDL_Texture *alien_texture;
    extern float *alien_aspect;
    extern SDL_FRect *bullet_src_rect;
    extern SDL_Texture *bullet_texture;
    extern float *bullet_aspect;
    extern SDL_FRect *player_src_rect;
    extern SDL_Texture *player_texture;
    extern float *player_aspect;

    void LoadImages(SDL_Renderer *renderer);
    void UnloadImages();
};
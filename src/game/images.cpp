#include "images.h"
#include "assets/alien.h"
#include "assets/alienf2.h"
#include "assets/bullet.h"
#include "assets/player.h"
#include "assets/playerf2.h"
#include "assets/zap1.h"
#include "assets/zap1f2.h"
#include "assets/zap2.h"
#include "assets/zap3.h"

SDL_Surface *images::alien_surface = nullptr;
SDL_Surface *images::alienf2_surface = nullptr;
SDL_Surface *images::bullet_surface = nullptr;
SDL_Surface *images::player_surface = nullptr;
SDL_Surface *images::playerf2_surface = nullptr;
SDL_Surface *images::zap1_surface = nullptr;
SDL_Surface *images::zap1f2_surface = nullptr;
SDL_Surface *images::zap2_surface = nullptr;
SDL_Surface *images::zap3_surface = nullptr;

SDL_Texture *images::alien_texture = nullptr;
SDL_Texture *images::alienf2_texture = nullptr;
SDL_Texture *images::bullet_texture = nullptr;
SDL_Texture *images::player_texture = nullptr;
SDL_Texture *images::playerf2_texture = nullptr;
SDL_Texture *images::zap1_texture = nullptr;
SDL_Texture *images::zap1f2_texture = nullptr;
SDL_Texture *images::zap2_texture = nullptr;
SDL_Texture *images::zap3_texture = nullptr;

SDL_FRect images::alien_src_rect;
SDL_FRect images::player_src_rect;
SDL_FRect images::bullet_src_rect;
SDL_FRect images::zap_src_rect;

float images::alien_aspect;
float images::bullet_aspect;
float images::player_aspect;

void images::LoadImages(SDL_Renderer *renderer)
{
    // surfaces
    images::alien_surface = utils::loadEmbeddedBMP(renderer, alien_bmp, alien_bmp_len);
    images::alienf2_surface = utils::loadEmbeddedBMP(renderer, alienf2_bmp, alienf2_bmp_len);
    images::bullet_surface = utils::loadEmbeddedBMP(renderer, bullet_bmp, bullet_bmp_len);
    images::player_surface = utils::loadEmbeddedBMP(renderer, player_bmp, player_bmp_len);
    images::playerf2_surface = utils::loadEmbeddedBMP(renderer, playerf2_bmp, playerf2_bmp_len);
    images::zap1_surface = utils::loadEmbeddedBMP(renderer, zap1_bmp, zap1_bmp_len);
    images::zap1f2_surface = utils::loadEmbeddedBMP(renderer, zap1f2_bmp, zap1f2_bmp_len);
    images::zap2_surface = utils::loadEmbeddedBMP(renderer, zap2_bmp, zap2_bmp_len);
    images::zap3_surface = utils::loadEmbeddedBMP(renderer, zap3_bmp, zap3_bmp_len);

    // textures
    images::alien_texture = SDL_CreateTextureFromSurface(renderer, images::alien_surface);
    images::alienf2_texture = SDL_CreateTextureFromSurface(renderer, images::alienf2_surface);
    images::bullet_texture = SDL_CreateTextureFromSurface(renderer, images::bullet_surface);
    images::player_texture = SDL_CreateTextureFromSurface(renderer, images::player_surface);
    images::playerf2_texture = SDL_CreateTextureFromSurface(renderer, images::playerf2_surface);
    images::zap1_texture = SDL_CreateTextureFromSurface(renderer, images::zap1_surface);
    images::zap1f2_texture = SDL_CreateTextureFromSurface(renderer, images::zap1f2_surface);
    images::zap2_texture = SDL_CreateTextureFromSurface(renderer, images::zap2_surface);
    images::zap3_texture = SDL_CreateTextureFromSurface(renderer, images::zap3_surface);

    // setting up the src sizes;
    float w, h;

    SDL_GetTextureSize(images::alien_texture, &w, &h);
    SDL_Log("Alien texture size W: %f H %f", w, h);
    images::alien_aspect = w / h;
    images::alien_src_rect = SDL_FRect{0, 0, w, h};

    SDL_GetTextureSize(images::player_texture, &w, &h);
    SDL_Log("Player texture size W: %f H %f", w, h);
    images::player_aspect = w / h;
    images::player_src_rect = SDL_FRect{0, 0, w, h};

    SDL_GetTextureSize(images::bullet_texture, &w, &h);
    SDL_Log("Bullet texture size W: %f H %f", w, h);
    images::bullet_aspect = w / h;
    images::bullet_src_rect = SDL_FRect{0, 0, w, h};

    SDL_GetTextureSize(images::zap1_texture, &w, &h);
    images::zap_src_rect = SDL_FRect{0, 0, w, h};
}

void images::UnloadImages()
{
    SDL_DestroySurface(images::alien_surface);
    SDL_DestroySurface(images::alienf2_surface);
    SDL_DestroySurface(images::bullet_surface);
    SDL_DestroySurface(images::player_surface);
    SDL_DestroySurface(images::playerf2_surface);
    SDL_DestroySurface(images::zap1_surface);
    SDL_DestroySurface(images::zap1f2_surface);
    SDL_DestroySurface(images::zap2_surface);
    SDL_DestroySurface(images::zap3_surface);

    SDL_DestroyTexture(images::alien_texture);
    SDL_DestroyTexture(images::alienf2_texture);
    SDL_DestroyTexture(images::bullet_texture);
    SDL_DestroyTexture(images::player_texture);
    SDL_DestroyTexture(images::playerf2_texture);
    SDL_DestroyTexture(images::zap1_texture);
    SDL_DestroyTexture(images::zap1f2_texture);
    SDL_DestroyTexture(images::zap2_texture);
    SDL_DestroyTexture(images::zap3_texture);
}
#include "images.h"
#include "assets/bullet.h"
#include "assets/player.h"
#include "assets/playerf2.h"
#include "assets/player2.h"
#include "assets/player2f2.h"
#include "assets/zap1.h"
#include "assets/zap1f2.h"
#include "assets/zap2.h"
#include "assets/zap3.h"
#include "assets/playerblink.h"
#include "assets/player2blink.h"

SDL_Surface *images::player2_surface = nullptr;
SDL_Surface *images::player2f2_surface = nullptr;

SDL_Surface *images::player_surface = nullptr;
SDL_Surface *images::playerf2_surface = nullptr;
SDL_Surface *images::zap1_surface = nullptr;
SDL_Surface *images::zap1f2_surface = nullptr;
SDL_Surface *images::zap2_surface = nullptr;
SDL_Surface *images::zap3_surface = nullptr;

SDL_Texture *images::player2_texture = nullptr;
SDL_Texture *images::player2f2_texture = nullptr;

SDL_Texture *images::player_texture = nullptr;
SDL_Texture *images::playerf2_texture = nullptr;
SDL_Texture *images::zap1_texture = nullptr;
SDL_Texture *images::zap1f2_texture = nullptr;
SDL_Texture *images::zap2_texture = nullptr;
SDL_Texture *images::zap3_texture = nullptr;

SDL_FRect images::player_src_rect;
SDL_FRect images::zap_src_rect;

float images::player_aspect;

size_t images_len = 1;

ImageData images[] = {
    ImageData{},
    ImageData{}
};

void images::LoadImages(SDL_Renderer *renderer)
{
    // surfaces
    images::player2_surface = utils::loadEmbeddedBMP(renderer, player2_bmp, player_bmp_len);
    images::player2f2_surface = utils::loadEmbeddedBMP(renderer, player2f2_bmp, playerf2_bmp_len);
    images::player_surface = utils::loadEmbeddedBMP(renderer, player_bmp, player_bmp_len);
    images::playerf2_surface = utils::loadEmbeddedBMP(renderer, playerf2_bmp, playerf2_bmp_len);
    images::zap1_surface = utils::loadEmbeddedBMP(renderer, zap1_bmp, zap1_bmp_len);
    images::zap1f2_surface = utils::loadEmbeddedBMP(renderer, zap1f2_bmp, zap1f2_bmp_len);
    images::zap2_surface = utils::loadEmbeddedBMP(renderer, zap2_bmp, zap2_bmp_len);
    images::zap3_surface = utils::loadEmbeddedBMP(renderer, zap3_bmp, zap3_bmp_len);

    // textures
    images::player2_texture = SDL_CreateTextureFromSurface(renderer, images::player2_surface);
    images::player2f2_texture = SDL_CreateTextureFromSurface(renderer, images::player2f2_surface);
    images::player_texture = SDL_CreateTextureFromSurface(renderer, images::player_surface);
    images::playerf2_texture = SDL_CreateTextureFromSurface(renderer, images::playerf2_surface);
    images::zap1_texture = SDL_CreateTextureFromSurface(renderer, images::zap1_surface);
    images::zap1f2_texture = SDL_CreateTextureFromSurface(renderer, images::zap1f2_surface);
    images::zap2_texture = SDL_CreateTextureFromSurface(renderer, images::zap2_surface);
    images::zap3_texture = SDL_CreateTextureFromSurface(renderer, images::zap3_surface);

    // setting up the src sizes;
    float w, h;

    SDL_GetTextureSize(images::player_texture, &w, &h);
    SDL_Log("Player texture size W: %f H %f", w, h);
    images::player_aspect = w / h;
    images::player_src_rect = SDL_FRect{0, 0, w, h};

    SDL_GetTextureSize(images::zap1_texture, &w, &h);
    images::zap_src_rect = SDL_FRect{0, 0, w, h};

    images[0].Load(renderer, playerblink_bmp, playerblink_bmp_len);
    images[1].Load(renderer, player2blink_bmp, player2blink_bmp_len);
}

void images::UnloadImages()
{
    SDL_DestroySurface(images::player2_surface);
    SDL_DestroySurface(images::player2f2_surface);
    SDL_DestroySurface(images::player_surface);
    SDL_DestroySurface(images::playerf2_surface);
    SDL_DestroySurface(images::zap1_surface);
    SDL_DestroySurface(images::zap1f2_surface);
    SDL_DestroySurface(images::zap2_surface);
    SDL_DestroySurface(images::zap3_surface);

    SDL_DestroyTexture(images::player2_texture);
    SDL_DestroyTexture(images::player2f2_texture);
    SDL_DestroyTexture(images::player_texture);
    SDL_DestroyTexture(images::playerf2_texture);
    SDL_DestroyTexture(images::zap1_texture);
    SDL_DestroyTexture(images::zap1f2_texture);
    SDL_DestroyTexture(images::zap2_texture);
    SDL_DestroyTexture(images::zap3_texture);

    for(size_t i = 0; i < images_len; i++)
    {
        images[i].Free();
    }
}

void ImageData::Load(SDL_Renderer *renderer, unsigned char bmp_data[], unsigned int bmp_len)
{
    this->surface = utils::loadEmbeddedBMP(renderer, bmp_data, bmp_len);
    this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
    SDL_DestroySurface(this->surface);
}

void ImageData::Free()
{
    SDL_DestroyTexture(this->texture);
}
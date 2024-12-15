#include "images.h"

void images::LoadImages(SDL_Renderer* renderer)
{
    images::alien = utils::loadEmbeddedBMP(renderer, alien_bmp, alien_bmp_len);
    images::bullet = utils::loadEmbeddedBMP(renderer, bullet_bmp, bullet_bmp_len);
    images::player = utils::loadEmbeddedBMP(renderer, player_bmp, player_bmp_len);
}

void images::UnloadImages()
{
}
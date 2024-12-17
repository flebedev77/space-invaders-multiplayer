#include "util.h"

namespace utils
{
    bool aabb(SDL_FRect *a, SDL_FRect *b)
    {
        return (
            a->x + a->w > b->x &&
            a->x < b->x + b->w &&
            a->y + a->h > b->y &&
            a->y < b->y + b->h);
    }

    SDL_Surface *loadEmbeddedBMP(SDL_Renderer *renderer, unsigned char bmp[], unsigned int bmpLength)
    {
        SDL_IOStream *io = SDL_IOFromMem(bmp, bmpLength); // Updated to SDL3 API
        if (!io)
        {
            std::cerr << "Error creating SDL_IO: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        // Load the image into an SDL_Surface from the memory
        SDL_Surface *surface = SDL_LoadBMP_IO(io, 1); // Use SDL3's updated function
        if (!surface)
        {
            std::cerr << "Error loading image: " << SDL_GetError() << std::endl;
            return nullptr;
        }

        return surface;
    }

    float lerp(float a, float b, float t)
    {
        return a + (b - a) * t;
    }

    std::string formatZerosString(uint16_t digitAmount, uint16_t &value)
    {
        if (value > std::pow(10, digitAmount) - 1)
        {
            value = 0;
        }
        std::string text = std::to_string(value);
        uint8_t textLen = std::strlen(text.c_str());
        uint8_t amtZeros = abs(digitAmount - textLen);
        for (uint8_t i = 0; i < amtZeros; i++)
        {
            text = "0" + text;
        }
        return text;
    }
}
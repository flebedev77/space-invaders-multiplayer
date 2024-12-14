#pragma once
#include "vector.h"
#include <SDL3/SDL.h>

namespace drawUtils {
    void DrawCircle(SDL_Renderer* renderer, Vector2 pos, float radius);
};
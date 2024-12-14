#pragma once
#include "vector2.h"
#include <SDL3/SDL.h>

namespace drawUtils {
    void DrawCircle(SDL_Renderer* renderer, Vector2 pos, float radius);
};
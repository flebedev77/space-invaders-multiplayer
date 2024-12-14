#include "drawutils.h"

void drawUtils::DrawCircle(SDL_Renderer* renderer, Vector2 pos, float radius) {
    int x = radius, y = 0;
    int decisionOver2 = 1 - x;  // Initial decision parameter
    float centerX = pos.x;
    float centerY = pos.y;

    while (x >= y) {
        // Draw all eight octants of the circle
        SDL_RenderPoint(renderer, centerX + x, centerY + y);  // Octant 1
        SDL_RenderPoint(renderer, centerX - x, centerY + y);  // Octant 2
        SDL_RenderPoint(renderer, centerX + x, centerY - y);  // Octant 3
        SDL_RenderPoint(renderer, centerX - x, centerY - y);  // Octant 4
        SDL_RenderPoint(renderer, centerX + y, centerY + x);  // Octant 5
        SDL_RenderPoint(renderer, centerX - y, centerY + x);  // Octant 6
        SDL_RenderPoint(renderer, centerX + y, centerY - x);  // Octant 7
        SDL_RenderPoint(renderer, centerX - y, centerY - x);  // Octant 8

        // Increment y, and adjust x based on the decision parameter
        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;  // Change in decision parameter for y
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;  // Change in decision parameter for x and y
        }
    }
}
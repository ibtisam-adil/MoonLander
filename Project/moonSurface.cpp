#include "moonSurface.h"

MoonSurface::MoonSurface() {
    ground = { 0, 500, 800, 100 }; // Ground at the bottom of the window
}

void MoonSurface::update(Rocket& rocket) {
    // Add logic to check for collisions or surface interaction
    if (rocket.y + 40 >= ground.y) {
        rocket.isLanded = true; // The rocket has hit the ground
    }
}

void MoonSurface::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // gray moon surface
    SDL_RenderFillRect(renderer, &ground);
}

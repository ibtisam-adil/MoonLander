#pragma once
#include <SDL.h>
#include "rocket.h"

class MoonSurface {
public:
    MoonSurface();
    void update(Rocket& rocket);
    void render(SDL_Renderer* renderer);
private:
    SDL_Rect ground;
};

#pragma once
#include <SDL.h>

class Rocket {
public:
    Rocket();
    void update();
    void render(SDL_Renderer* renderer);

    float x, y; // Position
    float velocityX, velocityY; // Velocity
    float thrust; // Thrust power
    bool isLanded;

private:
    void applyGravity();
    void applyThrust();
};

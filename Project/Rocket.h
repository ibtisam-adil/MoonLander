#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Vector2.h"
#include "LandscapeLine.h"

class Rocket {
public:
    Rocket(SDL_Renderer* renderer);
    ~Rocket();

    bool loadTexture(const char* path);
    void handleInput(const Uint8* keys);
    void checkCollision(const std::vector<LandscapeLine>& lines);
    bool lineIntersectsRocket(const LandscapeLine& line);
    bool pointIsBelowLine(const Vector2& point, const LandscapeLine& line);
    void land();
    void crash();
    void update(const std::vector<LandscapeLine>& lines);
    void render();
    void cleanup();

//private:
    Vector2 position;
    Vector2 velocity;
    float angle;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    bool landed;
    bool hasLandedOrCrashed;

    const float SCREEN_WIDTH = 800.0f; // Example SCREEN_WIDTH, adjust as needed
    const float ROTATION_SPEED = 2.0f; // Example ROTATION_SPEED, adjust as needed
    const float THRUST_POWER = 0.05f;   // Example THRUST_POWER, adjust as needed
    const float GRAVITY = 0.004f;        // Example GRAVITY, adjust as needed
};

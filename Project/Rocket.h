#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Vector2.h"
#include "LandscapeLine.h"
#include <SDL_ttf.h>

class Rocket {
public:
    Rocket(SDL_Renderer* renderer);
    ~Rocket();

    bool loadTexture(const char* path);
    void handleInput(const Uint8* keys, float deltaTime);
    void checkCollision(const std::vector<LandscapeLine>& lines);
    bool lineIntersectsRocket(const LandscapeLine& line);
    bool pointIsBelowLine(const Vector2& point, const LandscapeLine& line);
    void renderCrashMessages(TTF_Font* font, SDL_Renderer* renderer);
    void land();
    void crash();
    void update(const std::vector<LandscapeLine>& lines, float deltaTime);
    void render(TTF_Font* font);
    void cleanup();

    float getAltitude(const std::vector<LandscapeLine>& lines);
    float getTimeElapsed();
    int getFuel();
    Vector2 getVelocity();
    void reset();
    bool hasCrashed() const;
    bool hasLanded() const;
    void setFuel(int amount);

    std::vector<std::string> crashReasons;
    Vector2 position;
    Vector2 velocity;
    float angle;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    bool landed;
    bool hasLandedOrCrashed;
    float timeElapsed;

    int fuel;
    float thrustBuild;

    const float SCREEN_WIDTH = 1200.0f;
    const int SCREEN_HEIGHT = 800;
    const float ROTATION_SPEED = 2.0f;
    const float THRUST_POWER = 10.0f;
    const float GRAVITY = 0.01f;
    const float RIGHTWARD_VELOCITY = 0.3f;
    const float INITIAL_HORIZONTAL_SPEED = 30.0f;
    const float INITIAL_VERTICAL_SPEED = 12.0f;
};

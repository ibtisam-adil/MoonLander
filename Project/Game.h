#pragma once
#include <SDL.h>
#include <SDL_image.h>

// Airplane structure
struct Airplane {
    float x, y;    // Position
    float vx, vy;  // Velocity
    float angle;   // Rotation angle
    bool landingGear = false;

    Airplane() : x(0), y(0), vx(0), vy(0), angle(0), landingGear(false) {}

    void applyGravity();
    void update();
    void rotate(bool left);
    void throttleUp();
    void applyFlaps();
};

// Game class
class Game {
public:
    Game();
    ~Game();

    bool init();
    void handleInput();
    void update();
    void render();
    void run();
    void cleanup();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* airplaneTexture;
    bool running;
    Airplane plane;
};

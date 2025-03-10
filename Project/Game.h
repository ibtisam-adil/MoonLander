#pragma once
#include <SDL.h>

// Airplane structure
struct Airplane {
    float x, y;    // Position
    float vx, vy;  // Velocity
    float angle;   // Rotation angle

    void applyGravity();
    void update();
    void rotate(bool left);
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
    bool running;
    Airplane plane;
};

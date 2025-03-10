#include "Game.h"
#include <iostream>

// Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float GRAVITY = 0.1f;
const float ROTATION_SPEED = 5.0f;
const float MAX_SPEED = 5.0f;

// ------------------------------ Airplane Methods ------------------------------
// Gravity effect
void Airplane::applyGravity() {
    vy += GRAVITY;
}

// Update position
void Airplane::update() {
    // Apply velocity to position
    x += vx;
    y += vy;

    // Prevent plane from falling out of screen
    if (y > SCREEN_HEIGHT - 50) y = SCREEN_HEIGHT - 50;
}

// Rotate left/right
void Airplane::rotate(bool left) {
    angle += left ? -ROTATION_SPEED : ROTATION_SPEED;

    // Limit rotation range (optional)
    if (angle < -30) angle = -30;
    if (angle > 30) angle = 30;

    // Adjust velocity based on angle (simple physics)
    vx = angle * 0.05f;  // Small horizontal drift
    vy += angle * 0.02f; // Affect descent
}

// ------------------------------ Game Class Methods ------------------------------
Game::Game() : window(nullptr), renderer(nullptr), running(false) {
    // Set initial airplane state
    plane.x = SCREEN_WIDTH / 2;
    plane.y = 100;
    plane.vx = 0;
    plane.vy = 0;
    plane.angle = 0;
}

Game::~Game() {
    cleanup();
}

// Initialize SDL
bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    window = SDL_CreateWindow("Airplane Landing",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;

    running = true;
    return true;
}

// Handle input (arrow keys)
void Game::handleInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT) plane.rotate(true);
            if (event.key.keysym.sym == SDLK_RIGHT) plane.rotate(false);
        }
    }
}

// Update physics and game logic
void Game::update() {
    plane.applyGravity();
    plane.update();

    // Check if plane has landed
    if (plane.y >= SCREEN_HEIGHT - 100) {
        running = false;  // Game over
        std::cout << "Game Over! Plane landed/crashed." << std::endl;
    }
}

// Render the scene
void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
    SDL_RenderClear(renderer);

    // Draw runway
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 300, SCREEN_HEIGHT - 100, 900, SCREEN_HEIGHT - 100);

    // Draw airplane (as a rectangle for now)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect planeRect = { (int)plane.x, (int)plane.y, 40, 20 };
    SDL_RenderFillRect(renderer, &planeRect);

    SDL_RenderPresent(renderer);
}

// Game loop
void Game::run() {
    while (running) {
        handleInput();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }
}

// Cleanup SDL
void Game::cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
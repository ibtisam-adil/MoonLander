#include "game.h"
#include <iostream>

Game::Game(int screenWidth, int screenHeight)
    : window(nullptr), renderer(nullptr), rocket(nullptr), landscape(nullptr), running(true), viewX(0) {
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SDL2 Moon Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    landscape = new Landscape(SCREEN_WIDTH);
    rocket = new Rocket(renderer);
    if (!rocket->loadTexture("assets/rocket.png")) {
        cleanup();
        return false;
    }

    keys = SDL_GetKeyboardState(NULL);
    return true;
}

void Game::handleEvents() {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void Game::update(float deltaTime) {
    rocket->handleInput(keys, deltaTime);  // Pass deltaTime as the second argument
    rocket->update(landscape->lines, deltaTime); // Pass deltaTime to update function

    if (rocket->hasLandedOrCrashed) {
        SDL_Delay(2000); // Pause for 2 seconds before closing
        running = false;  // Stop the game
    }
}


void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    landscape->render(renderer, viewX);
    rocket->render();

    SDL_RenderPresent(renderer);
}

void Game::run() {

    Uint32 lastTime = SDL_GetTicks();

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        handleEvents();
        update(deltaTime);
        render();
        SDL_Delay(16);
    }
}

void Game::cleanup() {
    if (rocket) {
        rocket->cleanup();
        delete rocket;
        rocket = nullptr;
    }

    if (landscape) {
        delete landscape;
        landscape = nullptr;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

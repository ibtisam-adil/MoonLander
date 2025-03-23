#include "game.h"
#include <iostream>

Game::Game(int screenWidth, int screenHeight)
    : window(nullptr), renderer(nullptr), rocket(nullptr), landscape(nullptr), running(true), viewX(0), score(0) {
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

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("assets/Roboto.ttf", 24); // Change to a valid font file in your project
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
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

    renderHUD(renderer, *rocket);

    SDL_RenderPresent(renderer);
}

void Game::renderHUD(SDL_Renderer* renderer, Rocket& rocket) {
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    std::string altitudeText = "Altitude: " + std::to_string((int)rocket.getAltitude(landscape->lines));
    std::string vertSpeedText = "V Speed: " + std::to_string((int)rocket.getVelocity().y);
    std::string horSpeedText = "H Speed: " + std::to_string((int)rocket.getVelocity().x);
    std::string scoreText = "Score: " + std::to_string(score);
    std::string fuelText = "Fuel: " + std::to_string(rocket.getFuel());
    std::string timeText = "Time: " + std::to_string((int)rocket.getTimeElapsed());


    renderText(renderer, altitudeText, font, textColor, SCREEN_WIDTH - 200, 10);
    renderText(renderer, vertSpeedText, font, textColor, SCREEN_WIDTH - 200, 40);
    renderText(renderer, horSpeedText, font, textColor, SCREEN_WIDTH - 200, 70);
    renderText(renderer, scoreText, font, textColor, 20, 10);
    renderText(renderer, fuelText, font, textColor, 20, 40);
    renderText(renderer, timeText, font, textColor, 20, 70);
}

void Game::renderText(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect textRect = { x, y, surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

    if (font) { // Clean up font
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit(); // Quit SDL_ttf

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

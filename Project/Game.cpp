#include "game.h"
#include <iostream>

enum GameState { MENU, PLAYING, GAME_OVER };
GameState currentState = MENU;

Game::Game(int screenWidth, int screenHeight)
    : window(nullptr), renderer(nullptr), rocket(nullptr), landscape(nullptr), running(true),
    viewX(0), score(0), font(nullptr), keys(nullptr), event(0)
{
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

    font = TTF_OpenFont("assets/Roboto.ttf", 24);
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
        else if (event.type == SDL_MOUSEBUTTONDOWN && currentState == MENU) {
            currentState = PLAYING;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
            if (currentState == GAME_OVER) {
                restart(true);
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (currentState == PLAYING) {
        rocket->handleInput(keys, deltaTime);
        rocket->update(landscape->lines, deltaTime);

        if (rocket->getFuel() < 200) {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lowFuelTimer >= lowFuelFlickerInterval) {
                lowFuelMessageVisible = !lowFuelMessageVisible;
                lowFuelTimer = currentTime;
            }
        }

        if (rocket->hasLandedOrCrashed) {
            if (rocket->hasCrashed()) {
                if (rocket->getFuel() > 300) {
                    rocket->setFuel(rocket->getFuel() - 300);
                    renderMessage("300 fuel units lost");
                    restart(false);
                }
                else {
                    renderMessage("Out of fuel, game over!");
                    currentState = GAME_OVER;
                    gameOverTime = SDL_GetTicks();
                }
            }
            else if (rocket->hasLanded()) {
                score += 100;
                renderMessage("Landing Successful!");
                restart(false);
            }
        }
    }
    else if (currentState == GAME_OVER) {
        if (SDL_GetTicks() - gameOverTime > gameOverDelay) {
            restart(true);
            currentState = MENU;
        }
    }
}


void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (currentState == MENU) {
        renderText(renderer, "Click to Start", font, { 255, 255, 255, 255 }, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2);
        renderText(renderer, "Use Arrow Keys to Control", font, { 255, 255, 255, 255 }, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 40);
    }
    else if (currentState == PLAYING) {
        landscape->render(renderer, viewX);
        rocket->render();
        renderHUD(renderer, *rocket);

        if (lowFuelMessageVisible) {
            renderText(renderer, "Low Fuel!", font, { 255, 0, 0, 255 }, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2);
        }
    }
    else if (currentState == GAME_OVER) {
        renderText(renderer, "Game Over!", font, { 255, 255, 255, 255 }, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2);
        renderText(renderer, "Final Score: " + std::to_string(score), font, { 255, 255, 255, 255 }, SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 + 40);
    }

    SDL_RenderPresent(renderer);
}


void Game::renderMessage(const std::string& message) {
    SDL_Color color = { 255, 255, 255, 255 };
    renderText(renderer, message, font, color, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
}

void Game::restart(bool fullRestart) {
    rocket->reset();

    if (fullRestart) {
        score = 0;
        rocket->setFuel(2000);
        currentState = MENU;
    }

    std::cout << "Game Restarted" << std::endl;
}

void Game::renderHUD(SDL_Renderer* renderer, Rocket& rocket) {
    SDL_Color textColor = { 255, 255, 255, 255 };
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

    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

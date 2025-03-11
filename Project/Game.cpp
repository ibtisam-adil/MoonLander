#include "Game.h"
#include <iostream>

// Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float GRAVITY = 0.01f;
const float FORWARD_SPEED = 1.0f;
const float ROTATION_SPEED = 5.0f;
const float MAX_SPEED = 5.0f;

// ------------------------------ Airplane Methods ------------------------------
void Airplane::applyGravity() {
    vy += GRAVITY;
}

void Airplane::update() {
    applyGravity();

    x += vx;  // Forward movement based on rotation
    y += vy;  // Vertical movement

    // Prevent plane from falling out of screen
    if (y > SCREEN_HEIGHT - 50) y = SCREEN_HEIGHT - 50;
}


// Rotate left/right
void Airplane::rotate(bool left) {
    angle += left ? -ROTATION_SPEED : ROTATION_SPEED;

    // Limit rotation
    if (angle < -30) angle = -30;  // Nose up
    if (angle > 30) angle = 30;    // Nose down

    // Adjust velocity based on rotation
    float rad = angle * (M_PI / 180.0f);
    vx = cos(rad) * 2.0f;  // Forward thrust
    vy += sin(rad) * 0.5f; // Adjust descent rate
}


//void Airplane::rotate(bool left) {
//    angle += left ? -ROTATION_SPEED : ROTATION_SPEED;
//
//    // Limit rotation
//    if (angle < -30) angle = -30;  // Nose up
//    if (angle > 30) angle = 30;    // Nose down
//
//    // Adjust velocity based on rotation
//    float rad = angle * (M_PI / 180.0f);
//    vx = cos(rad) * 2.0f;  // Forward thrust
//    vy += sin(rad) * 0.5f; // Adjust descent rate
//}
//
//


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

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;

    // Load airplane texture
    SDL_Surface* loadedSurface = IMG_Load("C:/Users/Student/Desktop/plane_2_blue.png");
    if (!loadedSurface) {
        std::cout << "Error loading texture: " << IMG_GetError() << std::endl;
        return false;
    }


    airplaneTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);  // Free the surface after making a texture

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

void Game::update() {
    plane.applyGravity();
    plane.update();

    // Check if plane has landed
    if (plane.y >= SCREEN_HEIGHT - 100) {
        running = false;  
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

    // Define the destination rectangle where the airplane will be drawn
    SDL_Rect destRect = { (int)plane.x, (int)plane.y, 40, 20 };  // Adjust size accordingly

    // Set the center of rotation (middle of the image)
    SDL_Point center = { 20, 10 };  // Half of width and height (adjust based on image)

    // Render the airplane texture with rotation
    SDL_RenderCopyEx(renderer, airplaneTexture, nullptr, &destRect, plane.angle, &center, SDL_FLIP_NONE);

    SDL_RenderPresent(renderer);
}


//void Game::render() {
//    SDL_SetRenderDrawColor(renderer, 0, 0, 30, 255);
//    SDL_RenderClear(renderer);
//
//    // Draw runway
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawLine(renderer, 300, SCREEN_HEIGHT - 100, 900, SCREEN_HEIGHT - 100);
//
//    // Airplane size
//    int planeWidth = 40;
//    int planeHeight = 20;
//
//    // Calculate rotated points manually
//    float rad = plane.angle * (M_PI / 180.0f);  // Convert degrees to radians
//    float cosA = cos(rad);
//    float sinA = sin(rad);
//
//    // Define center of the plane
//    float cx = plane.x + planeWidth / 2;
//    float cy = plane.y + planeHeight / 2;
//
//    // Compute rotated corner positions
//    SDL_Point points[4] = {
//        {(int)(cx + (-planeWidth / 2) * cosA - (-planeHeight / 2) * sinA),
//         (int)(cy + (-planeWidth / 2) * sinA + (-planeHeight / 2) * cosA)},
//
//        {(int)(cx + (planeWidth / 2) * cosA - (-planeHeight / 2) * sinA),
//         (int)(cy + (planeWidth / 2) * sinA + (-planeHeight / 2) * cosA)},
//
//        {(int)(cx + (planeWidth / 2) * cosA - (planeHeight / 2) * sinA),
//         (int)(cy + (planeWidth / 2) * sinA + (planeHeight / 2) * cosA)},
//
//        {(int)(cx + (-planeWidth / 2) * cosA - (planeHeight / 2) * sinA),
//         (int)(cy + (-planeWidth / 2) * sinA + (planeHeight / 2) * cosA)},
//    };

//    // Draw the rotated plane
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//    for (int i = 0; i < 4; i++) {
//        SDL_RenderDrawLine(renderer, points[i].x, points[i].y, points[(i + 1) % 4].x, points[(i + 1) % 4].y);
//    }
//
//    SDL_RenderPresent(renderer);
//}



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
    SDL_DestroyTexture(airplaneTexture);  // Free the airplane texture
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

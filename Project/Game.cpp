#include "Game.h"
#include <iostream>

// Constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float BASE_GRAVITY = 0.002f; // Increased base gravity for better control feel
const float FORWARD_SPEED = 1.0f;
const float ROTATION_SPEED = 5.0f;
const float MAX_SPEED = 5.0f;
const float NOSE_DOWN_GRAVITY = 0.004f;  // Increased gravity when nose is down
const float NOSE_UP_GRAVITY = 0.001f;    // Reduced gravity when nose is up

// ------------------------------ Airplane Methods ------------------------------
// Apply gravity dynamically based on angle
void Airplane::applyGravity() {
    if (angle > 10) {
        vy += NOSE_DOWN_GRAVITY; // Nose down → Stronger gravity
    }
    else if (angle < -10) {
        vy += NOSE_UP_GRAVITY; // Nose up → Weaker gravity
    }
    else {
        vy += BASE_GRAVITY; // Default gravity
    }
}

// Rotate left/right and adjust movement
void Airplane::rotate(bool left) {
    angle += left ? -ROTATION_SPEED : ROTATION_SPEED;

    // Limit rotation range to prevent extreme nose-up behavior
    if (angle < -30) angle = -30;  // Nose up, limit at -30 degrees
    if (angle > 30) angle = 30;    // Nose down, limit at +30 degrees

    // Adjust velocity based on rotation
    float rad = angle * (M_PI / 180.0f);

    // Forward thrust based on rotation (vx)
    vx = cos(rad) * 2.0f;  // Forward speed

    // Vertical speed adjustment based on nose direction (vy)
    vy += sin(rad) * 0.3f;  // If nose is up, vy is reduced; nose down, vy is increased
}

// Update airplane physics
void Airplane::update() {
    applyGravity();  // Apply gravity constantly

    // Ensure forward movement at all times
    vx = FORWARD_SPEED;

    // Update position based on velocity
    x += vx;
    y += vy;

    // If the plane is facing up, reduce the descent rate (make it less negative, not positive)
    if (angle < 0) {  // Nose is up
        if (vy > 0) vy = 0;  // Prevent going up, but slow the descent
    }

    // Prevent plane from falling out of screen (lower boundary check)
    if (y > SCREEN_HEIGHT - 50) {
        y = SCREEN_HEIGHT - 50; // Clamp to the bottom of the screen
        vy = 0;  // Stop vertical movement if hitting the ground
    }

    // Prevent plane from going too high (upper boundary check)
    if (y < 0) {
        y = 0;
        vy = 0;
    }
}

// Apply throttle to increase speed gradually
void Airplane::throttleUp() {
    float acceleration = 0.1f; // Small forward push
    vx += acceleration;

    // Simulate engine struggle against gravity
    if (angle < 0) vy -= 0.005f; // If nose is up, reduce gravity effect slightly

    // Limit speed
    if (vx > MAX_SPEED) vx = MAX_SPEED;
}

// Apply flaps to generate lift but add drag
void Airplane::applyFlaps() {
    float flapLift = -0.05f;  // Small lift force (negative means upward)
    float drag = 0.98f;       // Reduce speed slightly to simulate drag

    vy += flapLift;  // Increase lift slightly
    vx *= drag;      // Reduce forward speed slightly due to drag

    // Prevent excessive climbing
    if (vy < -2.0f) vy = -2.0f;
}

// ------------------------------ Game Class Methods ------------------------------

Game::Game() : window(nullptr), renderer(nullptr), airplaneTexture(nullptr), running(false) {
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
    SDL_Surface* loadedSurface = IMG_Load("assets/plane_2_blue.png");
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
            if (event.key.keysym.sym == SDLK_UP) plane.throttleUp();
            if (event.key.keysym.sym == SDLK_SPACE) plane.applyFlaps();
            if (event.key.keysym.sym == SDLK_LEFT) plane.rotate(true);
            if (event.key.keysym.sym == SDLK_RIGHT) plane.rotate(false);
            if (event.key.keysym.sym == SDLK_g) plane.landingGear = !plane.landingGear;
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

    if (plane.y >= SCREEN_HEIGHT - 100) {
        if (!plane.landingGear) {
            std::cout << "Crash! You forgot to lower landing gear." << std::endl;
        }
        else {
            std::cout << "Successful landing!" << std::endl;
        }
        running = false;
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

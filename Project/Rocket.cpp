#include "rocket.h"
#include <iostream>

Rocket::Rocket() {
    x = 400; // Start in the middle of the screen
    y = 50;
    velocityX = 0;
    velocityY = 0;
    thrust = 0;
    isLanded = false;
}

void Rocket::update() {
    if (!isLanded) {
        applyGravity();
        applyThrust();
        y += velocityY;
        x += velocityX;

        if (y >= 500) { // Landed condition, assuming the ground is at y = 500
            isLanded = true;
        }
    }
}

void Rocket::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { (int)x, (int)y, 40, 40 };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red rocket
    SDL_RenderFillRect(renderer, &rect);
}

void Rocket::applyGravity() {
    velocityY += 0.1; // Gravity pulling the rocket down
}

void Rocket::applyThrust() {
    // For simplicity, let's allow the user to control thrust using arrow keys (up and down)
    if (thrust > 0) {
        velocityY -= 0.2; // Apply thrust to reduce speed
        thrust -= 0.1; // Decrease thrust
    }
}

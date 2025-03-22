#include "Landscape.h"
#include <SDL.h>  // For SDL_Renderer and related functions
#include <cstdlib>  // For rand()

// Constructor
Landscape::Landscape(int screenWidth) {
    scale = screenWidth / 600.0f;  // Adjust scaling to match the screen width
    setupData();
    adjustPoints();
    generateLines();
    generateStars();
}

// Render function for drawing landscape
void Landscape::render(SDL_Renderer* renderer, int viewX) {
    int offset = 0;

    // Ensure landscape covers the entire screen width
    while (viewX - offset > rightEdge) offset += rightEdge;
    while (viewX - offset < 0) offset -= rightEdge;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Render lines
    for (size_t i = 0; i < lines.size(); i++) {
        Vector2 p1 = { lines[i].p1.x + offset, lines[i].p1.y };
        Vector2 p2 = { lines[i].p2.x + offset, lines[i].p2.y };

        // Change color based on whether the line is landable
        if (lines[i].landable) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for landing zones
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for normal terrain
        }

        // Draw the line
        SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    }

    // Draw stars
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    for (const auto& star : stars) {
        float starX = star.x + offset;
        float starY = star.y;
        if (starX >= 0 && starX <= SCREEN_WIDTH) {
            SDL_RenderDrawPoint(renderer, starX, starY);
        }
    }
}

// Set up initial data points for the landscape
void Landscape::setupData() {
    points = {
        {0.5, 355.55}, {5.45, 355.55}, {6.45, 359.4}, {11.15, 359.4}, {12.1, 363.65},
        {14.6, 363.65}, {15.95, 375.75}, {19.25, 388}, {19.25, 391.9}, {21.65, 400},
        {28.85, 404.25}, {30.7, 412.4}, {33.05, 416.7}, {37.9, 420.5}, {42.7, 420.5},
        {47.4, 416.65}, {51.75, 409.5}, {56.55, 404.25}, {61.3, 400}, {63.65, 396.15},
        {68, 391.9}, {70.3, 388}, {75.1, 386.1}, {79.85, 379.95}, {84.7, 378.95},
        {89.05, 375.65}, {93.75, 375.65}, {98.5, 376.55}, {103.2, 379.95}, {104.3, 383.8},
        {107.55, 388}, {108.95, 391.9}, {112.4, 396.15}, {113.3, 400}, {117.1, 404.25},
        {121.95, 404.25}, {125.3, 396.3}, {128.6, 394.2}, {132.45, 396.15}, {135.75, 399.9},
        {138.15, 408.15}, {144.7, 412.4}, {146.3, 424.8}, {149.55, 436.65}, {149.55, 431.05},
        {154.35, 420.85}, {163.45, 420.85}, {168.15, 425.05}, {172.95, 431.75}, {175.45, 432.9},
        {179.7, 428.6}, {181.95, 424.8}, {186.7, 422.5}, {189.15, 412.4}, {191.55, 404.35},
        {196.35, 402.4}, {200.7, 398.1}, {205.45, 391.9}, {210.15, 383.8}, {212.55, 375.75},

        // Landing zone
        {216.0, 368.0}, {220.0, 368.0}, {224.0, 367.0}, {228.0, 358.0}, {232.0, 354.0},

        {236.0, 348.0}, {240.0, 342.0}, {243.0, 350.0}, {247.0, 340.0}, {250.0, 335.0},
        {253.0, 330.0}, {256.0, 325.0}, {260.0, 320.0}, {263.0, 328.0}, {267.0, 315.0},
        {270.0, 310.0}, {273.0, 305.0}, {276.0, 300.0}, {280.0, 295.0}, {283.0, 290.0},

        // Landing zone
        {285.0, 290.0}, {290.0, 290.0}, {295.0, 290.0}, {300.0, 285.0},

        {305.0, 290.0}, {308.0, 295.0}, {311.0, 300.0}, {315.0, 305.0}, {320.0, 310.0},
        {323.0, 315.0}, {327.0, 320.0}, {331.0, 325.0}, {335.0, 330.0}, {341.0, 335.0},

        // Another landing zone
        {344.0, 335.0}, {348.0, 335.0}, {351.0, 335.0},

        {360.0, 330.0}, {365.0, 325.0}, {369.0, 310.0}, {372.0, 315.0}, {375.0, 320.0},
        {380.0, 335.0}, {385.0, 335.0}, {395.0, 350.0}, {400.0, 353.0}, {405.0, 355.0},
        {420.0, 390.0}, {425.0, 395.0}, {430.0, 380.0}, {435.0, 370.0}, {440.0, 360.0},
        // Landing zone
        {445.0, 350.0}, {450.0, 350.0}, {455.0, 340.0}, {460.0, 320.0}, {465.0, 310.0},
        {470.0, 300.0}, {475.0, 290.0}, {480.0, 285.0}, {485.0, 280.0}, {490.0, 275.0},

        //// Final landing platform at the bottom
        {530.0, 325.0}, {538.0, 325.0},

        {550.0, 300.0}, {555.0, 302.0}, {560.0, 304.0}, {565.0, 295.0}, {570.0, 300.0},
        {575.0, 302.0}, {580.0, 304.0}, {585.0, 295.0}, {589.0, 295.0}, {592.0, 300.0},
        {595.0, 302.0}, {598.0, 304.0}, {601.0, 295.0}, {607.0, 295.0}, {609.0, 300.0}
    };
}

// Adjust points by scaling and positioning
void Landscape::adjustPoints() {
    float minX = points[0].x, maxX = points[0].x;
    float minY = points[0].y, maxY = points[0].y;

    // Scale and track min/max Y values
    for (auto& p : points) {
        p.x *= scale;
        p.y *= scale;
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
    }

    // Shift all points so that the lowest point (maxY) touches the bottom of the screen
    float yOffset = SCREEN_HEIGHT - maxY;
    for (auto& p : points) {
        p.x -= minX;  // Align leftmost point to x = 0
        p.y += yOffset; // Shift down so the lowest point is at SCREEN_HEIGHT
    }

    rightEdge = maxX - minX;
}

// Generate lines based on the points
void Landscape::generateLines() {
    const float minLandingWidth = 80.0f; // Reduced width to allow more landable zones

    for (size_t i = 1; i < points.size(); i++) {
        bool isFlat = (points[i - 1].y == points[i].y);

        // Allow more landing zones by reducing the width requirement
        if (isFlat && (points[i].x - points[i - 1].x >= minLandingWidth)) {
            lines.emplace_back(points[i - 1], points[i]);
            lines.back().landable = true;  // Make it a landing zone
            lines.back().multiplier = 2;   // Bonus for landing on these zones
        }
        else {
            lines.emplace_back(points[i - 1], points[i]);
        }
    }
}

// Generate stars based on lines
void Landscape::generateStars() {
    for (const auto& line : lines) {
        if (rand() % 10 < 1) { // 10% chance to generate a star
            float starX = line.p1.x;
            float starY = (rand() % SCREEN_HEIGHT);
            if (starY < line.p1.y && starY < line.p2.y) {
                stars.push_back({ starX, starY });
            }
        }
    }
}

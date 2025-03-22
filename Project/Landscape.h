#pragma once
#include <vector>
#include <SDL.h>
#include "Vector2.h"  // Include the Vector2 header if you have it.
#include "LandscapeLine.h"  // Include the LandscapeLine header if you have it.
#include "Star.h"  // Include the Star header if you have it.

class Landscape {
public:
    std::vector<Vector2> points;
    std::vector<LandscapeLine> lines;
    std::vector<Star> stars;

    float scale;
    float rightEdge;
    const int SCREEN_WIDTH = 1200;
    const int SCREEN_HEIGHT = 800;


    explicit Landscape(int screenWidth);

    void render(SDL_Renderer* renderer, int viewX);

private:
    void setupData();
    void adjustPoints();
    void generateLines();
    void generateStars();
};


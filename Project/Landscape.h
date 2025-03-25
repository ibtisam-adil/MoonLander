#pragma once
#include <vector>
#include <SDL.h>
#include "Vector2.h"  
#include "LandscapeLine.h"
#include "Star.h"

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


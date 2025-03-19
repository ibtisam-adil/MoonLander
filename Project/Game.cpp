//#include "game.h"
//#include <cstdlib>
//#include <ctime>
//
//#define TERRAIN_STEP 10
//#define MAX_HEIGHT_VARIATION 100
//#define LANDING_ZONE_WIDTH 40
//#define FLATNESS_PROBABILITY 0.2
//
//Game::Game(int screenWidth, int screenHeight)
//    : screenWidth(screenWidth), screenHeight(screenHeight), terrainWidth(screenWidth * 3), lastGeneratedX(0) {
//    srand(time(nullptr));
//}
//
//Game::~Game() {}
//
//void Game::Init() {
//    GenerateTerrain();
//}
//
//void Game::GenerateTerrain() {
//    terrainPoints.clear();
//    int baseHeight = screenHeight / 2;
//    int x = 0;
//
//    while (x < terrainWidth) {
//        int height = baseHeight + (rand() % MAX_HEIGHT_VARIATION - MAX_HEIGHT_VARIATION / 2);
//
//        if ((rand() / (float)RAND_MAX) < FLATNESS_PROBABILITY) {
//            for (int i = 0; i < LANDING_ZONE_WIDTH; i += TERRAIN_STEP) {
//                terrainPoints.push_back({ x + i, height });
//            }
//            x += LANDING_ZONE_WIDTH;
//        }
//        else {
//            terrainPoints.push_back({ x, height });
//            x += TERRAIN_STEP;
//        }
//    }
//}
//
//void Game::Render(SDL_Renderer* renderer) {
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    for (size_t i = 1; i < terrainPoints.size(); i++) {
//        SDL_RenderDrawLine(renderer, terrainPoints[i - 1].x, terrainPoints[i - 1].y,
//            terrainPoints[i].x, terrainPoints[i].y);
//    }
//}
//
//void Game::UpdateView(float rocketX) {
//    if (rocketX > lastGeneratedX - screenWidth * 0.2 || rocketX < lastGeneratedX - screenWidth * 0.8) {
//        GenerateTerrain();
//        lastGeneratedX = rocketX;
//    }
//}

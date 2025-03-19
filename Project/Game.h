//#pragma once
//
//#include <vector>
//#include <SDL2/SDL.h>
//
//class Game {
//public:
//    Game(int screenWidth, int screenHeight);  // Constructor
//    ~Game();
//
//    void Init();
//    void Run();  // Add this function in case it's used in your game loop
//    void GenerateTerrain();
//    void Render(SDL_Renderer* renderer);
//    void UpdateView(float rocketX);
//
//private:
//    int screenWidth, screenHeight;
//    int terrainWidth;
//    std::vector<SDL_Point> terrainPoints;
//    int lastGeneratedX;
//};
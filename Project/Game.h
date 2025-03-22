#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <SDL_image.h>
#include "Rocket.h"
#include "Landscape.h"

class Game {
public:
	Game(int screenWidth, int screenHeight);
	~Game();
	bool init();
	void run();
	void cleanup();

private:
	void handleEvents();
	void update();
	void render();

	SDL_Window* window;
	SDL_Renderer* renderer;
	Rocket* rocket;
	Landscape* landscape;

	bool running;
	SDL_Event event;
	const Uint8* keys;
	int viewX;
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 800;
};

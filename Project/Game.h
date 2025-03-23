#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <SDL_image.h>
#include "Rocket.h"
#include "Landscape.h"
#include <SDL_ttf.h>
#include <string>


class Game {
public:
	Game(int screenWidth, int screenHeight);
	~Game();
	bool init();
	void run();
	void cleanup();
	void renderHUD(SDL_Renderer* renderer, Rocket& rocket);
	void renderText(SDL_Renderer* renderer, const std::string& text, TTF_Font* font, SDL_Color color, int x, int y);
	void renderMessage(const std::string& message);
	void restart(bool fullRestart);

private:
	void handleEvents();
	void update(float deltaTime);
	void render();

	TTF_Font* font;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Rocket* rocket;
	Landscape* landscape;

	bool running;
	int score = 0;
	SDL_Event event;
	const Uint8* keys;
	int viewX;
	const int SCREEN_WIDTH = 1200;
	const int SCREEN_HEIGHT = 800;
	bool lowFuelMessageVisible = false;  
	Uint32 lowFuelTimer = 0;  // Timer to control flicker frequency
	const Uint32 lowFuelFlickerInterval = 500;  // Time in milliseconds for the flicker interval (adjustable)

};

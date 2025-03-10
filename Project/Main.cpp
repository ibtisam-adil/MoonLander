#define SDL_MAIN_HANDLED
#include "Game.h"

int main() {
    Game game;

    if (!game.init()) return -1;

    game.run();
    return 0;
}
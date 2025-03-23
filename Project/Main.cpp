#include "game.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;

int main(int argc, char* argv[]) {
    Game game(SCREEN_WIDTH, SCREEN_HEIGHT); 
    if (!game.init()) {
        return -1;
    }

    game.run();

    return 0;
}

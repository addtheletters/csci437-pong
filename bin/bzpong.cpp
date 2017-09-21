// Runs the game.

#include "PongGame.h"

int main(int argc, char** argv) {
    PongGame game;
    game.init();
    return game.gameLoop();
}


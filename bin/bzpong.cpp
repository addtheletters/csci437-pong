 
//
//  TODO write some description here
//

#include <SFML/Graphics.hpp>

#include "PongGame.h"

int main(int argc, char** argv) {
    PongGame game;
    game.init();
    return game.gameLoop();
}

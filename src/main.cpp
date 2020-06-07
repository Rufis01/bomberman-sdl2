#include <string>

#include "Game.h"

int main(int /*argc*/, char** /*argv[]*/)
{
    // init game
    bomberman::Game* game = new bomberman::Game(960, 544);
    // run game loop
    game->run();
    delete(game);   //Destroy or crash if gameover by enemy attack
    return 0;
}

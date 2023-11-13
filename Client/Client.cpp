#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    Game* game = Game::Instance();
    game->init();

    //Loop
    while (game->isOpen()) game->update();

    return 0;
}
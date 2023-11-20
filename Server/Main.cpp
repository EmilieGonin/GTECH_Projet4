#include <SFML/Graphics.hpp>
#include "Game.h"
#include "ServerClient.h"
#include "ServerWeb.h"
#include "JsonHandler.h"

int main(int ac, char const* av[])
{
	Game* game = Game::Instance();
	game->init();
	game->createImage();

	ServerClient serverClient;
	serverClient.init();
	//ServerWeb serverWeb;
	//serverWeb.init();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (game->hasWin())
		{
			//Send winner
			//j = JsonHandler(game->getCells(), game->getWinner());
			//serverClient.sendJson(j.getDump());

			//TODO->close window
		}
	}
}
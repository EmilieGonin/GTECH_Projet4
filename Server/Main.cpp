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

	//JsonHandler j(game->getCells());

	ServerClient serverClient;
	serverClient.init();
	//ServerWeb serverWeb;
	//serverWeb.init();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//serverClient.sendJson(j.getDump());

	/*if (game->hasWin())
	{
		j = JsonHandler(game->getCells(), game->getWinner());
		serverClient.sendJson(j.getDump());
	}*/
}
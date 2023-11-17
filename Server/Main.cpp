#include "ServerClient.h"
#include "ServerWeb.h"
#include "Game.h"
#include "JsonHandler.h"

int __cdecl main(void)
{
	Game* game = Game::Instance();
	game->init();

	//JsonHandler j(game->getCells());

	//ServerClient serverClient;
	//serverClient.init();
	ServerWeb serverWeb;
	serverWeb.init();

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
#include "ServerClient.h"
#include "ServerWeb.h"
#include "Game.h"
#include "JsonHandler.h"

int __cdecl main(void)
{
	//Game* game = Game::Instance();
	//game->init();

	//JsonHandler j(game->getCells());

	ServerClient serverClient;
	serverClient.init();
	//serverClient.sendJson(j.getDump());
	//ServerWeb serverWeb;
	//serverWeb.init();
}
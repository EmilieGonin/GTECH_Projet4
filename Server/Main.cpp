#include <SFML/Graphics.hpp>
#include <thread>
#include "Game.h"
#include "ServerClient.h"
#include "ServerWeb.h"
#include "JsonHandler.h"

void startWebServer();
void startClientServer();

int main(int ac, char const* av[])
{
	//Game* game = Game::Instance();
	//game->init();
	//game->createImage();

	//JsonHandler j(game->getCells());

	//std::thread(&startWebServer).detach();
	std::thread(&startClientServer).detach();


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//BOOL bRet;
	//MSG msg;
	//while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	//{
	//	if (bRet == -1)
	//	{
	//		// handle the error and possibly exit
	//	}
	//	else
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

	//serverClient.sendJson(j.getDump());

	/*if (game->hasWin())
	{
		j = JsonHandler(game->getCells(), game->getWinner());
		serverClient.sendJson(j.getDump());
	}*/

	WSACleanup();
}

void startWebServer() {
	ServerWeb serverWeb;
	serverWeb.init();
}

void startClientServer() {
	ServerClient serverClient;
	serverClient.init();

}
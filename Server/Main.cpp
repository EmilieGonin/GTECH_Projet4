#include <SFML/Graphics.hpp>
#include <thread>
#include "Game.h"
#include "ServerClient.h"
#include "ServerWeb.h"
#include "JsonHandler.h"

void startWebServer();
void startClientServer();

void startWebServer() {
	ServerWeb serverWeb;
	serverWeb.init();
}

void startClientServer() {
	ServerClient serverClient;
	serverClient.init();
}

int main(int ac, char const* av[])
{



	//std::thread(&startWebServer).detach();
	std::thread(&startClientServer).detach();

#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif

	Game* game = Game::Instance();
	game->init();
	game->createImage();



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

	game->reset();
	WSACleanup();

#ifdef _DEBUG
	_CrtMemState memStateEnd, memStateDiff{};
	_CrtMemCheckpoint(&memStateEnd);

	if (_CrtMemDifference(&memStateDiff, &memStateInit, &memStateEnd))
	{
		_CrtDumpMemoryLeaks();
		MessageBoxA(NULL, "MEMORY LEAKS", "DISCLAIMER", 0);
	}
#endif
	return 0;
}
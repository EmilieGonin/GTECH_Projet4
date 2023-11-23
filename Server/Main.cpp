#include <SFML/Graphics.hpp>
#include <thread>
#include "Game.h"
#include "ServerClient.h"
#include "ServerWeb.h"
#include "JsonHandler.h"
#include <windows.h>
#include <iostream>

DWORD WINAPI startClientServer(LPVOID lpParam) 
{
	ServerClient serverClient;
	serverClient.init();
	return 0;
}

DWORD WINAPI startWebServer(LPVOID lpParam) 
{
	ServerWeb serverWeb;
	serverWeb.init();
	return 0;
}

int main(int ac, char const* av[])
{
#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif

	HANDLE hThreadClient;
	DWORD threadIdClient;
	HANDLE hThreadWeb;
	DWORD threadIdWeb;

	Game* game = Game::Instance();
	game->init();
	game->createImage();

	 // Création du thread ClientServeur
	hThreadClient = CreateThread(NULL, 0, startClientServer, NULL, 0, &threadIdClient);
	if (hThreadClient == NULL) {
		std::cerr << "Erreur lors de la création du thread : " << GetLastError() << std::endl;
		return 1;
	}
	
	// Création du thread WebServeur
	hThreadWeb = CreateThread(NULL, 0, startWebServer, NULL, 0, &threadIdWeb);
	if (hThreadWeb == NULL) {
		std::cerr << "Erreur lors de la création du thread : " << GetLastError() << std::endl;
		return 1;
	}

	WaitForSingleObject(hThreadClient, INFINITE);// Attendre la fin du thread
	WaitForSingleObject(hThreadWeb, INFINITE);// Attendre la fin du thread
	
	CloseHandle(hThreadClient);	// Fermer le handle du thread
	CloseHandle(hThreadWeb);	// Fermer le handle du thread

	WSACleanup();
	game->reset();

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
#include "ServerClient.h"
#include <thread>
#include <windows.h>
#include "../WindowsProject1/framework.h"


Server::Server() {}
Server::~Server() {}

Server* Server::pServer = nullptr;

void Server::init()
{
	initHWND();
	initWSA();
	initSocket();
	listenClient();

	// cleanup
	//closesocket(ClientSocket);
	//WSACleanup();
}

void Server::initHWND(){}

void Server::initWSA()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("%s WSAStartup failed with error: %d\n", mName.c_str(), iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
}

void Server::initSocket()
{
	// Resolve the server address and port
	iResult = getaddrinfo(NULL, mPort.c_str(), &hints, &result);
	if (iResult != 0) {
		printf("%s getaddrinfo failed with error: %d\n", mName.c_str(), iResult);
		WSACleanup();
		return;
	}

	// Create a SOCKET for the server to listen for client connections.
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("%s socket failed with error: %ld\n", mName.c_str(), WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	printf("%s Socket initialized\n", mName.c_str());
}


void Server::listenClient()
{
	iResult = bind(ListenSocket, result->ai_addr, static_cast<int>(result->ai_addrlen));
	if (iResult == SOCKET_ERROR) {
		printf("%s bind failed with error: %d\n", mName.c_str(), WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("%s Bind successful.\n", mName.c_str());

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("%s listen failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	printf("%s Listening for clients...\n", mName.c_str());
	WSAAsyncSelect(ListenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);

	MSG msg;
	while (GetMessage(&msg, hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Server::accepteClient(SOCKET client) {}

LRESULT Server::WindowProc(HWND hWnd, UINT uMsg, WPARAM socket, LPARAM lParam) //static
{
	//pServer = reinterpret_cast<Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	//if (pServer)
	//	pServer->HandleWindowMessage(uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_SOCKET:
	{
		switch (LOWORD(lParam))
		{
		case FD_READ:
			pServer->HandleReadEvent(socket);
			break;
		case FD_ACCEPT:
			pServer->HandleAcceptEvent(socket);
			break;
		case FD_CLOSE:
			pServer->HandleCloseEvent(socket);
			break;
		default:
			break;
		}
		return 0; // Indique que le message a été traité
	}
	//pServer->handleClient(uMsg,socket, lParam);
	}
	return DefWindowProc(hWnd, uMsg, socket, lParam);
}

LRESULT Server::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	/*int requestID = static_cast<int>(wParam);
	std::string requestData(reinterpret_cast<const char*>(lParam));
	if (requestData.find("close") != std::string::npos)
	{
		printf("close connexion since you asked it");
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		closesocket(ClientSocket);
		WSACleanup();
	}*/


	/*switch (uMsg)
	{

	}*/

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Server::handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) {}

void Server::shutdownClient(SOCKET clientSocket)
{
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("%s shutdown failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(clientSocket);
		WSACleanup();
	}

	// Attendre quelques instants pour permettre la fermeture propre du socket
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	mPlayers.erase(clientSocket);

	//mPlayers.erase(std::remove_if(mPlayers.begin(), mPlayers.end(),
		//[clientSocket](SOCKET s) { return s == clientSocket; }), mPlayers.end());

	closesocket(clientSocket);

}

std::string Server::generateSessionID() const {
	// Implémente une logique pour générer un identifiant de session unique
	// Dans un contexte de production, tu pourrais utiliser une bibliothèque dédiée à la génération d'UUID ou d'autres méthodes de génération d'identifiants uniques.
	// Pour cette démo, un simple timestamp pourrait suffire.
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

	return "SessionID_" + std::to_string(timestamp);
}

void Server::HandleReadEvent(WPARAM socket)
{
	iResult = recv(socket, recvbuf, recvbuflen, 0);
	printf("%s Read event :\n %s\n", mName.c_str(), recvbuf);
	handleJson(socket, recvbuf);
}

void Server::HandleAcceptEvent(WPARAM socket)
{
	accepteClient(socket);
}

void Server::HandleCloseEvent(WPARAM wParam)
{
	//printf("Close event\n %lu\n", wParam);
}

void Server::sendJson(SOCKET client, std::string json)
{
	send(client, json.c_str(), json.size(), 0);
}

void Server::handleJson(SOCKET client, std::string dump)
{
	JsonHandler response;
	Game* game = Game::Instance();
	json json = json::parse(dump);
	int id = json["Id"];
	std::string playerId = json["Player"];
	std::pair<int, int> cell = json["Cell"];

	switch (id)
	{
	case 1: //Play cell
		//Check if it's player turn
	{
		bool error = game->getPlayerTurn() != playerId;
		if (!error)
		{
			game->updateCells(cell, playerId);

			//Check if player has win
			if (game->hasWin()) response = JsonHandler(game->getCells(), playerId);
			else response = JsonHandler(game->getCells(), game->getPlayerTurn(), error);
		}

		for (auto& player : mPlayers)
		{
			if (player.second != playerId && error) continue;
			sendJson(player.first, response.getDump());
		}
	}
		break;
	case 2: //Get cells after reconnect
		response = JsonHandler(game->getCells(), game->getPlayerTurn(), false);
		sendJson(client, response.getDump());
		break;
	default:
		break;
	}
}
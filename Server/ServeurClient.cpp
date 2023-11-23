#include "ServerClient.h"

ServerClient* ServerClient::mInstance = nullptr;

ServerClient::ServerClient() {}

ServerClient::~ServerClient()
{
	UnregisterClass(L"AsyncSelectWindowClassA", GetModuleHandle(NULL));
}

ServerClient* ServerClient::Instance()
{
	if (mInstance == nullptr) mInstance = new ServerClient();
	return mInstance;
}

void ServerClient::init()
{
	mPort = "1027";
	mName = "Serveur du jeu -";
	Server::init();
}

void ServerClient::initHWND()
{
	WNDCLASS wca = { 0 };
	wca.lpfnWndProc = WindowProc;
	wca.hInstance = GetModuleHandle(NULL);
	wca.lpszClassName = L"AsyncSelectWindowClassA";


	if (!RegisterClass(&wca)) {
		printf("%s RegisterClass failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassA", L"AsyncSelectWindowA", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("%s CreateWindowEx failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	printf("%s HWND created\n", mName.c_str());
}

void ServerClient::accepteClient(SOCKET client)
{
	Game* game = Game::Instance();

	// Accept a client socket
	client = accept(ListenSocket, NULL, NULL);
	if (client == INVALID_SOCKET && client == SOCKET_ERROR) {
		printf("%s accept failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("%s Client accepted.\n", mName.c_str());

	//Regarde si le client est d�ja enregistr� et le met en jeu ou spectate
	DispatchClient(game, client);

	if (mPlayers.size() == 2)
	{
		game->init();

		JsonHandler j(game->getCells(), game->getLastCell(), game->getPlayerTurn(), false);

		//Send cells to all players
		printf("%s Sending cells to players...\n", mName.c_str());
		std::string t = j.getDump();
		for (auto& player : mPlayers) send(player.first, j.getDump().c_str(), j.getDump().size(), 0);
	}

	WSAAsyncSelect(client, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
}

std::string ServerClient::GiveSessionID(SOCKET client)
{
	// Attribuer un identifiant de session au client
	std::string sessionID = generateSessionID();
	JsonHandler j(sessionID);
	printf("%s Sending session id : %s\n", mName.c_str(), sessionID.c_str());
	send(client, j.getDump().c_str(), j.getDump().size(), 0);
	Sleep(1000);
	return sessionID;
}

void ServerClient::DispatchClient(Game* game, SOCKET client)
{
	//Verifier si le joueurs est d�j� enregistrer
	bool enregistre = false;
	for (auto it = mAllClient.begin(); it != mAllClient.end(); ++it)
	{
		if (it->first == client)
		{
			if (mPlayers.size() < 2)
			{
				mPlayers[client] = it->second;
				game->addPlayer(it->second);
				printf("Client added to players.\n");
			}
			else {
				mSpectate[client] = it->second;
				printf("Client added to spectators.\n");
			}
			enregistre = true;
			break;
		}
	}

	//Si le Joueur n'�tait pas d�ja enregistr�
	if (!enregistre)
	{
		//Genere un ID et l'envoi au client
		std::string sessionID = GiveSessionID(client);
		mAllClient[client] = sessionID;

		if (mPlayers.size() < 2)
		{
			mPlayers[client] = sessionID;
			game->addPlayer(sessionID);
			printf("Client added to players.\n");
		}
		else 
		{
			mSpectate[client] = sessionID;
			printf("Client added to spectators.\n");
		}
	}
}

void ServerClient::HandleReadEvent(WPARAM socket)
{
	memset(recvbuf, 0, recvbuflen);
	iResult = recv(socket, recvbuf, recvbuflen, 0);
	printf("%s Read event :\n %s\n", mName.c_str(), recvbuf);
	handleJson(socket, recvbuf);
}

void ServerClient::handleJson(SOCKET client, std::string dump)
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
	{
		//Check if it's player turn
		std::string playerTurn = game->getPlayerTurn();
		bool error = game->getPlayerTurn() != playerId;
		if (!error)
		{
			game->updateCells(cell, playerId);

			//Check if player has win
			switch (game->hasWin()) {
			case -1: //No win
				response = JsonHandler(game->getCells(), game->getLastCell(), game->getPlayerTurn(), error);
				break;
			case 0: //Tie
				response = JsonHandler(game->getCells(), game->getLastCell(), "None");
				break;
			case 1: //Win
				response = JsonHandler(game->getCells(), game->getLastCell(), playerId);
				break;
			}
		}

		for (auto& player : mPlayers)
		{
			if (player.second != playerId && error) continue;
			sendJson(player.first, response.getDump());
		}
	}
	break;
	case 2: //Get cells after reconnect
		response = JsonHandler(game->getCells(), game->getLastCell(), game->getPlayerTurn(), false);
		sendJson(client, response.getDump());
		break;
	default:
		break;
	}
}

void ServerClient::sendJson(SOCKET client, std::string json)
{
	send(client, json.c_str(), json.size(), 0);
}

LRESULT ServerClient::WindowProc(HWND hWnd, UINT uMsg, WPARAM socket, LPARAM lParam) //static
{
	if (mInstance == nullptr) return 1;

	switch (uMsg) {
	case WM_SOCKET:
	{
		switch (LOWORD(lParam))
		{
		case FD_READ:
			mInstance->HandleReadEvent(socket);
			break;
		case FD_ACCEPT:
			mInstance->accepteClient(socket);
			break;
		case FD_CLOSE:
			mInstance->HandleCloseEvent(socket);
			break;
		default:
			break;
		}
		return 0; // Indique que le message a été traité
	}}

	return DefWindowProc(hWnd, uMsg, socket, lParam);
}

void ServerClient::HandleCloseEvent(WPARAM wParam)
{
	printf("Close event\n %lu\n", wParam);
}
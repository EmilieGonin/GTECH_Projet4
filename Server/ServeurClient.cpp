#include "ServerClient.h"

ServerClient::ServerClient() {}

void ServerClient::init()
{
	mPort = "1027";
	mName = "Serveur du jeu -";

	WNDCLASS wca = { 0 };
	wca.lpfnWndProc = WindowProc;
	wca.hInstance = GetModuleHandle(NULL);
	wca.lpszClassName = L"AsyncSelectWindowClassA";


	if (!RegisterClass(&wca)) {
		printf("%s RegisterClass failed: %d\n", mName.c_str(), GetLastError());
		return ;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassA", L"AsyncSelectWindowA", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("%s CreateWindowEx failed: %d\n", mName.c_str(), GetLastError());
		return ;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);
	pServer = reinterpret_cast<Server*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	printf("%s HWND created\n", mName.c_str());

	Server::init();
}

void ServerClient::accepteClient(SOCKET client)
{
	Game* game = Game::Instance();

	// Accept a client socket
	client = accept(ListenSocket, NULL, NULL);
	if (client == INVALID_SOCKET) {
		printf("%s accept failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	printf("%s Client accepted.\n", mName.c_str());

	//Regarde si le client est d�ja enregistr� et le met en jeu ou spectate
	DispatchClient(game, client);

	//TODO -> check if game has started
	if (mPlayers.size() == 2)
	{
		game->init();

		JsonHandler j(game->getCells(), mPlayers.begin()->second, false);

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

void ServerClient::handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//if (iResult > 0)
	//{
	//	printf("Bytes received: %d\n", uMsg);

	//	// Echo the buffer back to the sender
	//	iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	//	if (iSendResult == SOCKET_ERROR)
	//	{
	//		printf("send failed with error: %d\n", WSAGetLastError());
	//		closesocket(ClientSocket);
	//		WSACleanup();
	//	}
	//	printf("Bytes sent: %d\n", iSendResult);
	//}
	//else if (iResult == 0)
	//{
	//	//printf("Connection closing from server...\n");

	//}
	///*else
	//{
	//	printf("recv failed with error: %d\n", WSAGetLastError());

	//}*/
}
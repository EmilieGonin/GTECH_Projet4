#include "ServerClient.h"

ServerClient::ServerClient() {}

void ServerClient::init()
{
	mPort = "1027";
	Server::init();
}

void ServerClient::accepteClient(SOCKET client)
{
	Game* game = Game::Instance();

	// Accept a client socket
	client = accept(ListenSocket, NULL, NULL);
	if (client == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("Client accepted.\n");

	// Attribuer un identifiant de session au client
	std::string sessionID = generateSessionID();
	JsonHandler j(sessionID);
	printf("Sending session id : %s\n", sessionID.c_str());
	send(client, j.getDump().c_str(), j.getDump().size(), 0);
	Sleep(1000);

	//std::lock_guard<std::mutex> lock(clientsMutex);
	if (mPlayers.size() < 2)
	{
		mPlayers[client] = sessionID;
		game->addPlayer(sessionID);
		printf("Client added to players.\n");
	}
	else {
		//spectateur ?
		printf("Client added to spectators.\n");
	}

	//TODO -> check if game has started
	if (mPlayers.size() == 2)
	{
		game->init();

		JsonHandler j(game->getCells(), mPlayers.begin()->second, false);
		//Send cells to all players
		printf("Sending cells to players...\n");
		std::string t = j.getDump();
		for (auto& player : mPlayers) send(player.first, j.getDump().c_str(), j.getDump().size(), 0);
	}

	WSAAsyncSelect(client, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
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
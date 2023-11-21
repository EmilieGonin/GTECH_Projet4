#include "ServerClient.h"

ServerClient::ServerClient() {}

void ServerClient::init()
{
	mPort = "1027";
	Server::init();
}

void ServerClient::accepteClient()
{
		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
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
		send(ClientSocket, j.getDump().c_str(), j.getDump().size(), 0);

		//std::lock_guard<std::mutex> lock(clientsMutex);
		if (clientsPlayer.size() < 2)
		{
			//TODO -> add to game
			clientsPlayer.push_back(ClientSocket);
			printf("Client added to players.\n");
		}
		else {
			//spectateur ?
			printf("Client added to spectators.\n");
		}

		//TODO -> check if game has started
		if (clientsPlayer.size() == 2)
		{
			Game* game = Game::Instance();
			game->init();

			JsonHandler j(game->getCells());
			//Send cells
			printf("Sending cells to clients...\n");
			send(ClientSocket, j.getDump().c_str(), j.getDump().size(), 0);
		}
}

void ServerClient::handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (iResult > 0)
	{
		printf("Bytes received: %d\n", uMsg);

		// Echo the buffer back to the sender
		iSendResult = send(ClientSocket, recvbuf, iResult, 0);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}
		printf("Bytes sent: %d\n", iSendResult);
	}
	else if (iResult == 0)
	{
		//printf("Connection closing from server...\n");

	}
	/*else
	{
		printf("recv failed with error: %d\n", WSAGetLastError());

	}*/


}
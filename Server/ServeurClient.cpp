#include "ServerClient.h"

ServerClient::ServerClient()
{

};

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
		send(ClientSocket, sessionID.c_str(), sessionID.size(), 0);

		//std::lock_guard<std::mutex> lock(clientsMutex);
		if (clientsPlayer.size() < 2)
		{
			clientsPlayer.push_back(ClientSocket);
		}
		else {
			//spectateur ?

		}

		if (clientsPlayer.size() == 2)
		{
			Game* game = Game::Instance();
			game->init();

			JsonHandler j(game->getCells());
			send(ClientSocket, j.getDump().c_str(), j.getDump().size(), 0);
		}

		//std::pair<int, int> pair = { 0, 1 };

		//Test JSON - temp
		//j = JsonHandler(pair, 1);
		//if (j.getJson()["Id"] == 1) game->updateCells(j.getJson()["Cell"], j.getJson()["Player"]);
		//Fin test JSON

		//handleClient(ClientSocket, sessionID);

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

void ServerClient::handleJson(std::string dump)
{
	Game* game = Game::Instance();
	json json = json::parse(dump);
	int id = json["Id"];
	int playerId = json["Player"];

	switch (id)
	{
	case 1:
		//Check if it's player turn
		if (game->getPlayerTurn() == playerId) //
	case 2:
		//
	default:
		break;
	}
}
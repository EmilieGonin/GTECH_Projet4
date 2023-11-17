#include "ServerWeb.h"

ServerWeb::ServerWeb() { };

void ServerWeb::init()
{
	mPort = "8888";
	Server::init();
}

void ServerWeb::handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			//std::string ok = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\nHello, World!";

			// Analyser la requête HTTP
			std::string httpResponse = processHttpRequest();

			iSendResult = send(ClientSocket, httpResponse.c_str(), httpResponse.size(), 0);

			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				break; // Sortir de la boucle si l'envoi échoue
			}
		}
		else if (iResult == 0)
		{
			printf("Connection closing from server...\n");
			break; // Sortir de la boucle si la connexion est fermée
		}
		else
		{
			int error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK) {
				// Gérer l'erreur, mais ne pas fermer la connexion en cas de WSAEWOULDBLOCK
				printf("recv failed with error: %d\n", error);
				break; // Sortir de la boucle pour d'autres erreurs que WSAEWOULDBLOCK
			}
		}
	} while (iResult > 0);
}

std::string ServerWeb::processHttpRequest() 
{
	return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\nHello, World!";
}

void ServerWeb::accepteClient()
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

	WSAAsyncSelect(ClientSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	// Attribuer un identifiant de session au client
	//std::string sessionID = generateSessionID();
	//send(ClientSocket, sessionID.c_str(), sessionID.size(), 0);

	//std::lock_guard<std::mutex> lock(clientsMutex);
	//clients.push_back(ClientSocket);

	////handleClient(ClientSocket, sessionID);
	////Test JSON - temp
	//Game* game = Game::Instance();
	//game->init();

	//JsonHandler j(game->getCells());
	//send(ClientSocket, j.getDump().c_str(), j.getDump().size(), 0);

	//std::pair<int, int> pair = { 0, 1 };

	//j = JsonHandler(pair, 1);
	//if (j.getJson()["Id"] == 1) game->updateCells(j.getJson()["Cell"], j.getJson()["Player"]);
	//Fin test JSON

	//handleClient(ClientSocket, sessionID);
}

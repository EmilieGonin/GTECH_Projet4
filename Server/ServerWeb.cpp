#include "ServerWeb.h"

ServerWeb::ServerWeb() { };

void ServerWeb::init()
{
	mPort = "8888";
	Server::init();
}

void ServerWeb::handleClient(SOCKET clientSocket, const std::string& sessionID) {
	do {

		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			std::string ok = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\nHello, World!";

			// Analyser la requête HTTP
			std::string httpResponse = processHttpRequest();

			iSendResult = send(clientSocket, httpResponse.c_str(), httpResponse.size(), 0);

			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(clientSocket);
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
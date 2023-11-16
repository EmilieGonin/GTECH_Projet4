#include "ServerWeb.h"

ServerWeb::ServerWeb() { };

void ServerWeb::init()
{
	mPort = "8888";
	Server::init();
}

void ServerWeb::handleClientWeb(SOCKET clientSocket, const std::string& sessionID) {
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			std::string httpRequest(recvbuf);

			// Analyser la requête HTTP
			std::string httpResponse = processHttpRequest(httpRequest);

			//printf("Bytes received: %s\n", sessionID.c_str(), recvbuf);

			// Envoyer la réponse HTTP
			iSendResult = send(ClientSocket, httpResponse.c_str(), httpResponse.size(), 0);

			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
			}
			//printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
		{
			printf("Connection closing from server...\n");

		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
		}
	} while (iResult > 0);
}

std::string ServerWeb::processHttpRequest(const std::string& httpRequest) 
{

	return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 13\r\n\r\nHello, World!";

}
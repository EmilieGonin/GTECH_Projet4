#include "ServerClient.h"

ServerClient::ServerClient()
{
	
};

void ServerClient::init()
{
	mPort = "1027";
	Server::init();
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
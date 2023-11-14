#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <thread>


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1027"
#define ADDRESS "localhost"
#define DEFAULT_BUFLEN 512
#define HEARTBEAT_INTERVAL 100 // Ou toute autre valeur que tu pr�f�res, en millisecondes

void handleHeartbeat(SOCKET clientSocket);


int main(int ac, char const* av[])
{
	int res;
	WSADATA wsaData;
	addrinfo* result = NULL, * connection = NULL, address;
	SOCKET ClientSocket = INVALID_SOCKET;

	const char* sendbuf = "";
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];

	ZeroMemory(&address, sizeof(address));
	address.ai_family = AF_INET;
	address.ai_socktype = SOCK_STREAM;
	address.ai_protocol = IPPROTO_TCP;

	if (res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed: %d\n", res);
		return 1;
	}

	// Resolve the server address and port
	if (res = getaddrinfo(ADDRESS, DEFAULT_PORT, &address, &result) != 0)
	{
		printf("getaddrinfo failed: %d\n", res);
		WSACleanup();
		return 1;
	}

	// Set the address and port to connect the server
	connection = result;
	ClientSocket = socket(connection->ai_family, connection->ai_socktype, connection->ai_protocol);

	if (res = connect(ClientSocket, connection->ai_addr, (int)connection->ai_addrlen) == SOCKET_ERROR)
	{
		printf("Unable to connect to server 1! Error code: %d\n", WSAGetLastError());

		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ClientSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server! Error code: %d\n", WSAGetLastError());
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		WSACleanup();
		return 1;
	}

	// Lancer la gestion des c�urs dans un thread s�par�
	std::thread heartbeatThread(handleHeartbeat, ClientSocket);
	heartbeatThread.detach();

	if (res = send(ClientSocket, sendbuf, (int)strlen(sendbuf), 0) == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", res);


	if (res = shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	do {
		res = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (res > 0)
			printf("Bytes received: %d\n", res);
		else if (res == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (res > 0);



	return 0;
}

void handleHeartbeat(SOCKET clientSocket) {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));

		// Envoie un message de c�ur au serveur
		send(clientSocket, "Heartbeat", sizeof("Heartbeat"), 0);
	}
}
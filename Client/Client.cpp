#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "1027"
#define ADDRESS "localhost"
#define DEFAULT_BUFLEN 512


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
		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	if (ClientSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

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
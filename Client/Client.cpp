#include "Client.h"

Client::Client()
{
	createInvisibleWindow();
	initClientSocket();
	//Sleep(2000);
	connectClientServer();
}

Client::~Client() {}

int Client::createInvisibleWindow()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"AsyncSelectWindowClass";

	if (!RegisterClass(&wc)) {
		printf("RegisterClass failed: %d\n", GetLastError());
		return 1;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClass", L"AsyncSelectWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("CreateWindowEx failed: %d\n", GetLastError());
		return 1;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	printf("HWND created\n");
}

int Client::initClientSocket()
{
	ZeroMemory(&address, sizeof(address));
	address.ai_family = AF_INET;
	address.ai_socktype = SOCK_STREAM;
	address.ai_protocol = IPPROTO_TCP;

	if (res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed: %d\n", res);
		return 1;
	}

	if (res = getaddrinfo(ADDRESS, DEFAULT_PORT, &address, &result) != 0)
	{
		printf("getaddrinfo failed: %d\n", res);
		WSACleanup();
		return 1;
	}

	printf("Socket initialized\n");
}

int Client::connectClientServer()
{
	printf("Connecting to server...\n");
	connection = result;
	ClientSocket = socket(connection->ai_family, connection->ai_socktype, connection->ai_protocol);

	//connect(ClientSocket, connection->ai_addr, (int)connection->ai_addrlen);
	if ((res = connect(ClientSocket, connection->ai_addr, (int)connection->ai_addrlen)) == SOCKET_ERROR)
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

	printf("Client connected\n");
	WSAAsyncSelect(ClientSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	//std::string data = JsonHandler(1).getDump();
	//clientSendData(data);
}

int Client::clientSendData(std::string data)
{
	res = send(ClientSocket, data.c_str(), (int)strlen(sendbuf), 0);
	if (res  == SOCKET_ERROR || res == INVALID_SOCKET)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	//printf(sendbuf);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	printf("Bytes Sent: %ld\n", res);
}

//int Client::clientDisconnect()
//{
//	/*if (res = shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR)
//	{
//		printf("shutdown failed: %d\n", WSAGetLastError());
//		closesocket(ClientSocket);
//		WSACleanup();
//		return 1;
//	}*/
//
//	//do {
//		res = recv(ClientSocket, recvbuf, recvbuflen, 0);
//		if (res > 0)
//			printf("Bytes received: %s\n", recvbuf);
//		else if (res == 0)
//			printf("Connection closed\n");
//		//else
//			//printf("recv failed: %d\n", WSAGetLastError());
//	//} while (true);
//
//	return 0;
//}

LRESULT Client::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) //static
{
	Client* client= reinterpret_cast<Client*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	switch (uMsg)
	{
	case WM_SOCKET:
	{
		switch (LOWORD(lParam))
		{
		case FD_READ:
			client->HandleReadEvent(wParam);
			break;
		case FD_CLOSE:
			client->HandleCloseEvent(wParam);
			break;
		default:
			break;
		}
		return 0; // Indique que le message a été traité
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Client::HandleWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}

void Client::HandleReadEvent(WPARAM wParam)
{
	//printf("Read event\n" + wParam);
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

	printf("Read event\n %s\n", recvbuf);
}

void Client::HandleAcceptEvent(WPARAM wParam)
{
	printf("Accept event\n %lu\n", wParam);
}

void Client::HandleCloseEvent(WPARAM wParam)
{
	printf("Close event\n %lu\n", wParam);
}
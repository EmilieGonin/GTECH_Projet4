#include "Client.h"

Client::Client()
{

}

Client::~Client()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int Client::createInvisibleWindow()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"AsyncSelectWindowClass";

	if (!RegisterClass(&wc)) {
		printf("RegisterClass failed: %d\n", GetLastError());
		return 1;
	}

	HWND hWnd = CreateWindowEx(0, L"AsyncSelectWindowClass", L"AsyncSelectWindow", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("CreateWindowEx failed: %d\n", GetLastError());
		return 1;
	}

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);
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
}

int Client::connectClientServer()
{
	connection = result;
	ClientSocket = socket(connection->ai_family, connection->ai_socktype, connection->ai_protocol);

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
}





int Client::clientSendData()
{
	if ((res = send(ClientSocket, sendbuf, (int)strlen(sendbuf), 0)) == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}
	printf(sendbuf);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	printf("Bytes Sent: %ld\n", res);
}

int Client::clientDisconnect()
{
	if (res = shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}


	/*res = recv(ClientSocket, recvbuf, recvbuflen, 0);
	if (res > 0)
		printf("Bytes received: %d\n", res);
	else if (res == 0)
		printf("Connection closed\n");*/
	//else
		//printf("recv failed: %d\n", WSAGetLastError());


	return 0;
}

int main(int ac, char const* av[])
{
	Client c;

	c.createInvisibleWindow();
	c.initClientSocket();
	c.connectClientServer();
	c.clientSendData();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0 )) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	c.clientDisconnect();

	return 0;
}
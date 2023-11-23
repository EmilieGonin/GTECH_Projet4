#include "ServerWeb.h"

ServerWeb* ServerWeb::mInstance = nullptr;

ServerWeb::ServerWeb() {}

ServerWeb::~ServerWeb()
{
	UnregisterClass(L"AsyncSelectWindowClassB", GetModuleHandle(NULL));
}

ServerWeb* ServerWeb::Instance()
{
	if (mInstance == nullptr) mInstance = new ServerWeb();
	return mInstance;
}

void ServerWeb::init()
{
	mPort = "8888";
	mName = "Serveur web -";

	Server::init();
}

void ServerWeb::initHWND()
{
	WNDCLASS wcb = { 0 };
	wcb.lpfnWndProc = WindowProc;
	wcb.hInstance = GetModuleHandle(NULL);
	wcb.lpszClassName = L"AsyncSelectWindowClassB";

	if (!RegisterClass(&wcb)) {
		printf("%s RegisterClassB failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassB", L"AsyncSelectWindowB", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("%s CreateWindowEx failed: %d\n", mName.c_str(), GetLastError());
		return;
	}

	ShowWindow(hWnd, SW_NORMAL);
	UpdateWindow(hWnd);

	printf("%s HWND created\n", mName.c_str());

	//SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	//mInstance = reinterpret_cast<ServerWeb*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

void ServerWeb::accepteClient(SOCKET client)
{
	// Accept a client socket
	client = accept(ListenSocket, NULL, NULL);
	if (client == INVALID_SOCKET) {
		printf("%s accept failed with error: %d\n", mName.c_str(), WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	printf("%s Client accepted.\n", mName.c_str());

	mPlayers[client] = generateSessionID(); //Replace with spectators map

	WSAAsyncSelect(client, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
}

std::string ServerWeb::processHttpRequest()
{
	std::ifstream file(imagePath, std::ios::binary);

	std::ostringstream imageStream;
	imageStream << file.rdbuf();

	std::string imageData = imageStream.str();
	std::string base64 = base64_encode(imageData);

	std::string html = "HTTP / 1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Tic-Tac-Toe</h1><img src=\"data:image/png;base64," + base64 + "\" />";

	return html;
}

void ServerWeb::showHTML()
{
	for (auto& client : mPlayers) send(client.first, processHttpRequest().c_str(), processHttpRequest().size(), 0);
}

void ServerWeb::HandleReadEvent(WPARAM socket)
{
	iResult = recv(socket, recvbuf, recvbuflen, 0);
	printf("%s Read event :\n %s\n", mName.c_str(), recvbuf);
	showHTML();
}

LRESULT ServerWeb::WindowProc(HWND hWnd, UINT uMsg, WPARAM socket, LPARAM lParam) //static
{
	if (mInstance == nullptr) return 1;

	switch (uMsg) {
	case WM_SOCKET:
	{
		switch (LOWORD(lParam))
		{
		case FD_READ:
			mInstance->HandleReadEvent(socket);
			break;
		case FD_ACCEPT:
			mInstance->accepteClient(socket);
			break;
		case FD_CLOSE:
			mInstance->HandleCloseEvent(socket);
			break;
		default:
			break;
		}
		return 0; // Indique que le message a été traité
	}}

	return DefWindowProc(hWnd, uMsg, socket, lParam);
}

void ServerWeb::HandleCloseEvent(WPARAM wParam)
{
	//printf("Close event\n %lu\n", wParam);
}
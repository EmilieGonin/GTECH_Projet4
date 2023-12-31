#include "Window.h"
#include "Client.h"

Client::Client() {}

Client::~Client() {}

void Client::reset()
{
	Window* w = Window::Instance(this);
	w->restart();

	DestroyWindow(hWnd);

	if (!UnregisterClass(L"AsyncSelectWindowClassC", GetModuleHandle(NULL))) {
		printf("UnregisterClass failed: %d\n", GetLastError());
		return;
	}

	printf("HWND destroyed and class unregistered\n");
}

int Client::init()
{
	if (initHWND() == 1) return 1;
	if (initSocket() == 1) return 1;
	//Sleep(2000);
	//if (connectClientServer() == 1) return 1;
	return 0;
}

int Client::initHWND()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"AsyncSelectWindowClassC";

	if (!RegisterClass(&wc)) {
		printf("RegisterClass failed: %d\n", GetLastError());
		return 1;
	}

	hWnd = CreateWindowEx(0, L"AsyncSelectWindowClassC", L"AsyncSelectWindowC", 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
	if (hWnd == NULL) {
		printf("CreateWindowEx failed: %d\n", GetLastError());
		return 1;
	}

	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	ShowWindow(hWnd, SW_HIDE);
	UpdateWindow(hWnd);

	printf("HWND created\n");
	return 0;
}

int Client::initSocket()
{
	if (res = WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("WSAStartup failed: %d\n", res);
		return 1;
	}

	ZeroMemory(&address, sizeof(address));
	address.ai_family = AF_INET;
	address.ai_socktype = SOCK_STREAM;
	address.ai_protocol = IPPROTO_TCP;

	if (res = getaddrinfo(ADDRESS, DEFAULT_PORT, &address, &result) != 0)
	{
		printf("getaddrinfo failed: %d\n", res);
		WSACleanup();
		return 1;
	}

	ClientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ClientSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	printf("Socket initialized\n");
	return 0;
}

int Client::connectClientServer()
{
	printf("Connecting to server...\n");
	if ((res = connect(ClientSocket, result->ai_addr, (int)result->ai_addrlen)) == (SOCKET_ERROR | INVALID_SOCKET))
	{
		printf("Unable to connect to server!\n");
		freeaddrinfo(result);
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	printf("Client connected\n");
	freeaddrinfo(result);
	WSAAsyncSelect(ClientSocket, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	return 0;
}

int Client::clientSendData(std::string data)
{
	res = send(ClientSocket, data.c_str(), data.size(), 0);
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

int Client::clientDisconnect()
{
	if (res = shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR)
	{
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	closesocket(ClientSocket);
	reset();
	init();
	return 0;
}

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
		return 0; // Indique que le message a �t� trait�
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
	memset(recvbuf, 0, recvbuflen);
	//TODO -> remove select to read header then body for size
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	printf("Read event :\n %s\n", recvbuf);
	handleJson(recvbuf);
}

void Client::HandleCloseEvent(WPARAM wParam)
{
	printf("Close event\n %lu\n", wParam);
}

void Client::sendJson(std::string json)
{
	send(ClientSocket, json.c_str(), json.size(), 0);
}

void Client::handleJson(std::string dump)
{
	JsonHandler response;
	Window* window = Window::Instance(this);
	json j = json::parse(dump);
	int id = j["Id"];
	int error = j["ErrorCode"];

	switch (id)
	{
	case 3: //Get cells after connect or play
		if (window->getCurrentScene() != GAME) window->changeScene(GAME);
		if (error == 0)
		{
			window->initTurnsList(j["Cells"], j["LastCell"]);
			window->initCells(j["Cells"]);
			window->resetTurn(j["Player"] == mPlayerId);
		}
		break;
	case 4: //Get cells and winner
		window->initCells(j["Cells"]);
		window->setWinner(j["Player"]);
		window->changeScene(END_GAME);
		DeleteData();
		clientDisconnect();
		break;
	case 5: //Get session id
		mPlayerId = j["Player"];
		if (IsDataExist())
		{
			printf("Datas already exist.\n");
			json data = ReadData();
			mPlayerId = data["Player"];
			sendJson(data.dump());
		}
		else WriteData(j["Player"]);

		window->setPlayer(mPlayerId);
		break;
	default:
		break;
	}
}

void Client::WriteData(std::string sessionID)
{
	printf("Writing new datas...\n");
	json mJson;
	std::string mDump;

	mJson["ErrorCode"] = 0;
	mJson["JsonType"] = NOTIF;
	mJson["Id"] = 6;
	mJson["Player"] = sessionID;

	mDump = mJson.dump();

	// Ouvrir un fichier en écriture
	std::ofstream fichier("donnees.json");

	// Écrire l'objet JSON dans le fichier
	fichier << mDump;

	// Fermer le fichier
	fichier.close();

	printf("New datas saved.\n");
}

json Client::ReadData()
{
	printf("Reading existing datas...\n");

	// Ouvrir un fichier en lecture
	std::ifstream fichier("donnees.json");

	// Créer un objet JSON
	json donnees;

	// Vérifier si le fichier est bien ouvert
	if (fichier.is_open()) {
		// Lire l'objet JSON depuis le fichier
		donnees = json::parse(fichier);
		//fichier >> donnees;

		// Fermer le fichier
		fichier.close();
	}
	else {
		std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
	}

	return donnees;
}

void Client::DeleteData()
{
	if (std::remove("donnees.json") == 0) puts("Fichier supprime avec succes");
	else perror("Erreur lors de la suppression du fichier");
}

bool Client::IsDataExist()
{
	std::ifstream fichier("donnees.json");
	return (bool)fichier;
}
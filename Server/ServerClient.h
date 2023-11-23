#pragma once
#include "Server.h"
#include "ServerWeb.h"

class ServerClient : public Server {

private:
	ServerClient();
	~ServerClient();
	static ServerClient* mInstance;

	std::string GiveSessionID(SOCKET client);
	void DispatchClient(Game* game, SOCKET client);

protected:
	void accepteClient(SOCKET);
	void handleJson(SOCKET, std::string dump);
	void sendJson(SOCKET, std::string);
	void initHWND()override;

public:
	static ServerClient* Instance();
	void init() override;
	void HandleReadEvent(WPARAM wParam) override;
	void HandleCloseEvent(WPARAM wParam) override;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
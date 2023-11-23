#pragma once
#include "Server.h"

class ServerClient : public Server {

private: 
	std::string GiveSessionID(SOCKET client);
	void DispatchClient(Game* game, SOCKET client);

protected:
	void handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void accepteClient(SOCKET) override;

public:
	ServerClient();
	void init() override;
};
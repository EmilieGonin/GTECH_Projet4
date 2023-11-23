#pragma once
#include "Server.h"

class ServerClient : public Server {

private: 
	std::string GiveSessionID(SOCKET client);
	void DispatchClient(Game* game, SOCKET client);

protected:
	void accepteClient(SOCKET) override;
	void handleJson(SOCKET, std::string dump);
	void initHWND()override;

public:
	ServerClient();
	~ServerClient();
	void init() override;
	void HandleReadEvent(WPARAM wParam) override;
};
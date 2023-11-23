#pragma once
#include "Server.h"

class ServerClient : public Server {

protected:
	void accepteClient(SOCKET) override;
	void handleJson(SOCKET, std::string dump);

public:
	ServerClient();
	~ServerClient();
	void init() override;
	void HandleReadEvent(WPARAM wParam) override;
};
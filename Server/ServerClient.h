#pragma once
#include "Server.h"

class ServerClient : public Server {

protected:
	void handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void accepteClient() override;

public:
	ServerClient();
	void init() override;
};
#pragma once
#include "Server.h"

class ServerClient : public Server {
private:
	void handleJson(std::string);

protected:
	void handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void accepteClient() override;

public:
	ServerClient();
	void init() override;
};
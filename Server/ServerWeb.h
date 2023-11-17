#pragma once
#include "Server.h"

class ServerWeb: public Server  {
private:
	void handleClient(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	std::string processHttpRequest();

public:
	ServerWeb();
	void init() override;
};
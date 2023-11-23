#pragma once
#include "Server.h"
#include <string>
#include <fstream>
#include <sstream>
#include "base64.h"

class ServerWeb : public Server {
private:
    void accepteClient(SOCKET) override;
    void showHTML(SOCKET);

    std::string processHttpRequest();
    std::string imagePath = "game.png";

public:
    ServerWeb();
    void init() override;
    void HandleReadEvent(WPARAM wParam) override;
};
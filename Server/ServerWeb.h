#pragma once
#include "Server.h"
#include <string>
#include <fstream>
#include <sstream>
#include "base64.h"

class ServerWeb : public Server {
private:
    ServerWeb();
    static ServerWeb* mInstance;
    void accepteClient(SOCKET) override;

    std::string processHttpRequest();
    std::string imagePath = "game.png";

public:
    static ServerWeb* Instance();
    void init() override;
    void HandleReadEvent(WPARAM wParam) override;
    void showHTML();
};
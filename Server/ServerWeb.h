#pragma once
#include "Server.h"
#include <string>
#include <fstream>
#include <sstream>
#include "base64.h"

class ServerWeb : public Server {
private:
    ServerWeb();
    ~ServerWeb();
    static ServerWeb* mInstance;
    void accepteClient(SOCKET);

    std::string processHttpRequest();
    std::string imagePath = "game.png";

protected:
    void initHWND() override;

public:
    static ServerWeb* Instance();
    void init() override;
    void showHTML(SOCKET);
    void HandleReadEvent(WPARAM wParam) override;
    void HandleCloseEvent(WPARAM wParam) override;

    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
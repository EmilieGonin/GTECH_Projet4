#include "Window.h"
#include "Client.h"

int main(int ac, char const* av[])
{
	Window w;
	Client c;

	c.createInvisibleWindow();
	c.initClientSocket();
	c.connectClientServer();
	c.clientSendData("test");

	//c.clientDisconnect();
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {


		TranslateMessage(&msg);
		DispatchMessage(&msg);
		//c.clientSendData();
		
		w.initTextFirstMenu();
		//w.initTextSecondMenu();
		w.update();

	}

	return 0;
}
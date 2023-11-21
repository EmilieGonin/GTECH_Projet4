#include "Window.h"
#include "Client.h"

int main(int ac, char const* av[])
{
	Window w;
	Client c;

	if (c.init() == 1)
	{
		printf("Error during client initialization.");
		return 1;
	}

	//std::pair<int, int> cell = { 0, 0 };
	//JsonHandler j(cell, 1);
	//c.clientSendData(j.getDump());
	//c.clientDisconnect();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		//w.initTextFirstMenu();
		//w.initTextSecondMenu();
		//w.update();
	}

	return 0;
}
#include "Window.h"
#include "Client.h"

int main(int ac, char const* av[])
{
	Window* w = Window::Instance();
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

	//w->changeScene(Window::MAIN_MENU);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
		/*switch (w->getCurrentScene())
		{
		case Window::MAIN_MENU:
			break;
		case Window::NAME_MENU:
			break;
		}*/
		//w->initTextFirstMenu();
		//w->initTextSecondMenu();
		w->update();

		if (!w->hasPlayed() && w->hasSelectedCell())
		{
			//TODO -> add playerId
			JsonHandler j(w->play(), w->getPlayer());
			c.clientSendData(j.getDump());
		}
	}
	return 0;
}
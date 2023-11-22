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
		case Window::GAME:
			break;
		}*/
		w->initTextFirstMenu();
		}

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
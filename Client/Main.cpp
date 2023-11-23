#include "Window.h"
#include "Client.h"

int main(int ac, char const* av[])
{
	Client c;
	Window* w = Window::Instance(&c);

	if (c.init() == 1)
	{
		printf("Error during client initialization.");
		//return 1;
	}

	//c.clientDisconnect();

	w->changeScene(Window::MAIN_MENU);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (w->update() == 1) return 1;

		if (!w->hasPlayed() && w->hasSelectedCell())
		{
			//TODO -> add playerId
			JsonHandler j(w->play(), w->getPlayer());
			c.clientSendData(j.getDump());
		}
	}
	return 0;
}
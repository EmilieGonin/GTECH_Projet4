#include "Window.h"
#include "Client.h"

int main(int ac, char const* av[])
{
	Window w;
	/*Client c;

	c.createInvisibleWindow();
	c.initClientSocket();
	c.connectClientServer();
	c.clientSendData();
	c.clientDisconnect();*/

	w.initTextMenu();
	while (w.isOpen()) w.update();

	return 0;
}
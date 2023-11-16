#include "ServerClient.h"
#include "ServerWeb.h"

int __cdecl main(void)
{
	ServerClient serverClient;
	serverClient.init();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
			
	}
	//ServerWeb serverWeb;
	//serverWeb.init();
}
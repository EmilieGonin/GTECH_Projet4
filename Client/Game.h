#pragma once
#include "Window.h"

class Game
{
public:
	static Game* Instance();
	void init();
	void update();
	bool hasWin();

	inline bool isOpen() { return mWindow->isOpen(); };

private:
	Game();
	static Game* mInstance;
	static Window* mWindow;
};

//Game* game = Game::Instance();
//game->init();

//Loop
//while (game->isOpen()) game->update();
#pragma once
#include "Window.h"

//struct cell {
//	sf::Shape* shape;
//	int player;
//};

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
	std::map<std::pair<int, int>, struct cell> mCells;
};
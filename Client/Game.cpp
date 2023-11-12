#include "Game.h"

Game* Game::mInstance = nullptr;
Window* Game::mWindow = nullptr;

Game::Game() { };

Game* Game::Instance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Game();
		mWindow = new Window();
	}

	return mInstance;
}

void Game::init()
{
	int cellNumber = 9;
	int cellSize = 150;
	int line = 0;
	int column = 0;

	for (size_t i = 1; i <= cellNumber; i++)
	{
		sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
		shape->setPosition(sf::Vector2f(cellSize * line, cellSize * column));

		if (i % 2 == 0)
		{
			shape->setFillColor(sf::Color::Green);
		} else shape->setFillColor(sf::Color::Red);

		line++;
		if (i % 3 == 0) line = 0, column++;

		mWindow->addCell(shape);
	}
}

void Game::update()
{
	mWindow->update();
}
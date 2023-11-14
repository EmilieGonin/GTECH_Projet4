#include "Game.h"
#include "Window.h"
#include "SFML/Graphics/Color.hpp"

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
		shape->setPosition(sf::Vector2f(cellSize * line + 150, cellSize * column + 150));

		if (i % 2 == 0)
		{
			shape->setFillColor(sf::Color::White);
		} else shape->setFillColor(sf::Color::Blue);

		mWindow->addCell({ line, column }, shape);

		line++;
		if (i % 3 == 0) line = 0, column++;
	}
}

void Game::update()
{
	mWindow->update();
	if (hasWin()) mWindow->Finito();
}

bool Game::hasWin()
{
	std::map<std::pair<int, int>, struct cell> cells = mWindow->getCells();
	int player = mWindow->getTurn() % 2 == 0 ? 1 : 2;

	for (size_t i = 0; i < 3; i++)
	{
		//Columns
		if (cells[{i, 0}].player == player && cells[{i, 1}].player == player && cells[{i, 2}].player == player) return true; 

		//Lines
		if (cells[{0, i}].player == player && cells[{1, i}].player == player && cells[{2, i}].player == player) return true;

		//Diagonales
		if (cells[{0, 0}].player == player && cells[{1, 1}].player == player && cells[{2, 2}].player == player) return true;
		if (cells[{0, 2}].player == player && cells[{1, 1}].player == player && cells[{2, 0}].player == player) return true;
	}

	return false;
}
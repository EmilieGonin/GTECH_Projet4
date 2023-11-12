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
	sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(100, 100));
	shape->setFillColor(sf::Color::Green);
	mWindow->addShape(shape);
}

void Game::update()
{
	mWindow->update();
}
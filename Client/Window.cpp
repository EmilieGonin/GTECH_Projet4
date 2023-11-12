#include "Window.h"

Window::Window()
{
	mWindow = new sf::RenderWindow(sf::VideoMode(800, 800), "Tic-tac-toe");
}

void Window::update()
{
	sf::Event event;
	while (mWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) mWindow->close();
	}

	mWindow->clear();
	for (auto& shape : mShapes) mWindow->draw(*shape);
	mWindow->display();
}

void Window::addShape(sf::Shape* shape)
{
	mShapes.push_back(shape);
}
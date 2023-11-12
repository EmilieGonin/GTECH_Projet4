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
		if (event.type == sf::Event::MouseButtonReleased) checkCollision(event);
	}

	mWindow->clear();
	for (auto& shape : mCells) mWindow->draw(*shape.first);
	for (auto& shape : mShapes) mWindow->draw(*shape);
	mWindow->display();
}

void Window::addShape(sf::Shape* shape)
{
	mShapes.push_back(shape);
}

void Window::addCell(sf::Shape* shape)
{
	mCells[shape] = true;
}

void Window::checkCollision(sf::Event e)
{
	sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	for (auto& shape : mCells)
	{
		if (!shape.second) continue; //If the shape has already been selected
		sf::FloatRect bounds = shape.first->getGlobalBounds();

		if (bounds.contains(pos))
		{
			shape.second = false; //Shape has now been selected
			addPlayer(bounds.getPosition());
			return;
		}
	}
}

void Window::addPlayer(sf::Vector2f position)
{
	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(position);

	if (mTurn % 2 == 0)
	{
		shape->setFillColor(sf::Color::Green);
	} else shape->setFillColor(sf::Color::Red);

	addShape(shape);
	mTurn++;
}
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

	if (!endGame)
	{
		mWindow->clear();
		for (auto& cell : mCells) mWindow->draw(*cell.second.shape);
		for (auto& shape : mShapes) mWindow->draw(*shape);
		mWindow->display();
	}
}

void Window::addShape(sf::Shape* shape)
{
	mShapes.push_back(shape);
}

void Window::addCell(std::pair<int, int> pos, sf::Shape* shape)
{
	struct cell newCell;
	newCell.shape = shape;
	newCell.player = 0;

	mCells[pos] = newCell;
}

void Window::Finito()
{
	endGame = true;
	if (mTurn % 2 == 0)	mWindow->setTitle("Red Win !");
	else mWindow->setTitle("Green Win !");
}

void Window::checkCollision(sf::Event e)
{
	if (!endGame)
	{
		sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

		for (auto& cell : mCells)
		{
			if (cell.second.player != 0) continue; //If the shape has already been selected
			sf::FloatRect bounds = cell.second.shape->getGlobalBounds();

			if (bounds.contains(pos))
			{
				cell.second.player = mTurn % 2 == 0 ? 2 : 1; //Shape has now been selected
				addPlayerShape(bounds.getPosition());
				return;
			}
		}
	}
}

void Window::addPlayerShape(sf::Vector2f position)
{
	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(position);

	if (mTurn % 2 == 0)shape->setFillColor(sf::Color::Green);
	else shape->setFillColor(sf::Color::Red);

	addShape(shape);
	mTurn++;
}
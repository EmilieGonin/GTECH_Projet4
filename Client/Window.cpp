#include "Window.h"

Window::Window()
{
	mWindow = new sf::RenderWindow(sf::VideoMode(800, 800), "Tic-tac-toe");
}

Window::~Window()
{
	for (auto& shape : mShapes) delete shape;
	for (auto& text : mTexts) delete text;
	for (auto& cell : mCells) delete cell.second.shape;
}

void Window::update()
{
	while (mWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) mWindow->close();
		if (event.type == sf::Event::MouseButtonReleased)
		{
			checkCollision(event);
			checkTextClick();
		}
		menuNameEnter();
	}

	changeMenuColor();

	if (!endGame)
	{
		mWindow->clear();
		for (auto& cell : mCells) mWindow->draw(*cell.second.shape);
		for (auto& shape : mShapes) mWindow->draw(*shape);
		if (hasEnterName) {
			mEnterName.setString("Enter your name: " + mName);
			mWindow->draw(mEnterName);
		}
		for (auto& text : mTexts) mWindow->draw(*text);
		for (auto& text : mTextMenu) mWindow->draw(*text);
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

void Window::checkTextClick()
{
	// Récupère la position du clic de souris
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	// Parcours des textes pour vérifier si l'un d'eux a été cliqué
	for (auto& text : mTextMenu)
	{
		// Récupère les limites de la zone occupée par le texte
		sf::FloatRect bounds = text->getGlobalBounds();

		// Vérifie la collision avec la position du clic de souris
		if (bounds.contains(mousePosition))
		{
			// Actions spécifiques au texte cliqué
			if (text->getString() == "Quit")
			{
				// Quitte le jeu
				mWindow->close();
			}
		}
	}
}


void Window::initTextFirstMenu()
{
	mFontTitle.loadFromFile("valoon.ttf");
	mFont.loadFromFile("arial.ttf");

	sf::Vector2i localPosition = sf::Mouse::getPosition(*mWindow);

	sf::Text* text = new sf::Text();
	text->setFont(mFontTitle);
	text->setString("Tic-Tac-Toe");
	text->setCharacterSize(80);
	text->setPosition(150, 100);
	text->setFillColor(sf::Color(31, 222, 190));
	mTexts.push_back(text);

	text = new sf::Text();
	text->setFont(mFont);
	text->setString("New Game");
	text->setCharacterSize(50);
	text->setPosition(300, 300);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Join");
	text->setCharacterSize(50);
	text->setPosition(300, 450);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Quit");
	text->setCharacterSize(50);
	text->setPosition(300, 600);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);
}

void Window::initTextSecondMenu()
{
	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();
	mEnterName.setFont(mFont);
	mEnterName.setCharacterSize(40);
	mEnterName.setPosition(200, 100);
	mEnterName.setFillColor(sf::Color::White);

	mFont.loadFromFile("arial.ttf");
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Let's go!");
	text->setCharacterSize(40);
	text->setPosition(300, 300);
	text->setFillColor(sf::Color::White);
	mTexts.push_back(text);

}

void Window::menuNameEnter()
{
	if (event.type == sf::Event::TextEntered && hasEnterName) {
		if (event.text.unicode < 128) {
			if (event.text.unicode == 13) {
				hasEnterName = false;
			}
			else if (event.text.unicode == 8) {
				if (!mName.empty()) {
					mName.pop_back();
				}
			}
			else {
				mName += static_cast<char>(event.text.unicode);
			}
		}
	}
}

void Window::changeMenuColor()
{
	sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	for (auto& text : mTextMenu)
	{
		auto gb = text->getGlobalBounds();
		if (gb.contains(pos)) text->setFillColor(sf::Color(222, 31, 63));
		else text->setFillColor(sf::Color::White);
	}
}
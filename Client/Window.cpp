#include "Window.h"

Window* Window::mInstance = nullptr;

Window::Window()
{
	mWindow = new sf::RenderWindow(sf::VideoMode(mWidth, mLength), "Tic-tac-toe");
	mSelectedCell = { -1, -1 };
	mHasPlayed = false;
}

Window::~Window()
{
	for (auto& shape : mShapes) delete shape;
	for (auto& text : mTexts) delete text;
	for (auto& cell : mCells) delete cell.second.shape;
}

Window* Window::Instance()
{
	if (mInstance == nullptr) mInstance = new Window();
	return mInstance;
}

void Window::update()
{
	while (mWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) mWindow->close();
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (!hasSelectedCell()) checkCollision(event);
			//checkTextClick();
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

void Window::initCells(std::map<std::pair<int, int>, std::string> cells)
{
	//Init cells when client connect for the first time
	int cellNumber = 9;
	int cellSize = 150;
	int line = 0;
	int column = 0;

	for (size_t i = 1; i <= cellNumber; i++)
	{
		sf::RectangleShape* shape = new sf::RectangleShape(sf::Vector2f(cellSize, cellSize));
		shape->setPosition(sf::Vector2f(cellSize * line + 150, cellSize * column + 150));

		if (i % 2 == 0) shape->setFillColor(sf::Color::White);
		else shape->setFillColor(sf::Color::Blue);

		cell newCell = { shape, {line, column}, cells[{line, column}] };
		mCells[{line, column}] = newCell;

		if (!newCell.player.empty()) addPlayerShape(shape->getPosition(), newCell.player);

		line++;
		if (i % 3 == 0) line = 0, column++;
	}
}

void Window::resetTurn()
{
	mHasPlayed = false;
	mSelectedCell = { -1, -1 };
}

std::pair<int, int> Window::play()
{
	mHasPlayed = true;
	return mSelectedCell;
}

void Window::addShape(sf::Shape* shape)
{
	mShapes.push_back(shape);
}

void Window::addCell(std::pair<int, int> pos, sf::Shape* shape)
{
	struct cell newCell;
	newCell.shape = shape;
	newCell.player = "";

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
			if (!cell.second.player.empty()) continue; //If the shape has already been selected
			sf::FloatRect bounds = cell.second.shape->getGlobalBounds();

			if (bounds.contains(pos))
			{
				cell.second.player = mTurn % 2 == 0 ? 2 : 1; //Shape has now been selected
				mSelectedCell = cell.second.pos;
				addPlayerShape(bounds.getPosition(), cell.second.player);
				return;
			}
		}
	}
}

void Window::addPlayerShape(sf::Vector2f position, std::string player)
{
	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(position);

	if (player == mPlayerId)shape->setFillColor(sf::Color::Green);
	else shape->setFillColor(sf::Color::Red);

	addShape(shape);
	mTurn++;
}

/*void Window::checkTextClick()
{
	// R�cup�re la position du clic de souris
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	// Parcours des textes pour v�rifier si l'un d'eux a �t� cliqu�
	for (auto& text : mTextMenu)
	{
		// R�cup�re les limites de la zone occup�e par le texte
		sf::FloatRect bounds = text->getGlobalBounds();

		// V�rifie la collision avec la position du clic de souris
		if (bounds.contains(mousePosition))
		{
			// Actions sp�cifiques au texte cliqu�
			if (text->getString() == "Quit")
			{
				// Quitte le jeu
				mWindow->close();
			}
			else if (text->getString() == "Let's go!")
			{
				// Lance le jeu
				changeScene(GAME);
			}
		}
	}
}*/

/*void Window::changeScene(SceneState newState)
{
	mShapes.clear();
	mTexts.clear();
	mTextMenu.clear();
	mCells.clear();

	currentScene = newState;

	switch (currentScene)
	{
	case Window::MAIN_MENU:
		initTextFirstMenu();
		break;
	case Window::GAME:
		break;
	}
}*/

void Window::initTextFirstMenu()
{
	mFontTitle.loadFromFile("valoon.ttf");
	mFont.loadFromFile("arial.ttf");

	sf::Vector2i localPosition = sf::Mouse::getPosition(*mWindow);

	// Tic-Tac-Toe text
	sf::Text* text = new sf::Text();
	text->setFont(mFontTitle);
	text->setString("Tic-Tac-Toe");
	text->setCharacterSize(80);
	text->setPosition(mWidth / 5.3, mLength / 8);
	text->setFillColor(sf::Color(31, 222, 190));
	mTexts.push_back(text);

	// Enter name text
	mFont.loadFromFile("arial.ttf");
	mEnterName.setFont(mFont);
	mEnterName.setCharacterSize(40);
	mEnterName.setPosition(mWidth / 4.7, mLength / 2.6);
	mEnterName.setFillColor(sf::Color::White);

	// New Game text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Play");
	text->setCharacterSize(50);
	text->setPosition(300, 450);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	// Join text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Join");
	text->setCharacterSize(50);
	text->setPosition(300, 550);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	// Quit text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Quit");
	text->setCharacterSize(50);
	text->setPosition(300, 600);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);
}

void Window::menuNameEnter()
{
	if (event.type == sf::Event::TextEntered && hasEnterName) {
		//R�cup�re les valeurs de la table ASCII
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
	//R�cup�re la position de la souris sur la fen�tre
	sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	//Boucle sur notre texte
	for (auto& text : mTextMenu)
	{
		auto gb = text->getGlobalBounds();
		if (gb.contains(pos)) text->setFillColor(sf::Color(222, 31, 63));
		else text->setFillColor(sf::Color::White);
	}
}
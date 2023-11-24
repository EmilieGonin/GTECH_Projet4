#include "Window.h"

Window* Window::mInstance = nullptr;

Window::Window()
{
	mWindow = new sf::RenderWindow(sf::VideoMode(mWidth, mLength), "Tic-tac-toe");
	mSelectedCell = { -1, -1 };
	mHasPlayed = false;
	hasError = false;
}

Window::~Window()
{
	for (auto& shape : mShapes) delete shape;
	for (auto& text : mTexts) delete text;
	for (auto& cell : mCells) delete cell.second.shape;
}

Window* Window::Instance(Client* mClient)
{
	if (mInstance == nullptr) mInstance = new Window();
	mInstance->client = mClient;
	return mInstance;
}

int Window::update()
{
	while (mWindow->pollEvent(event))
	{
		menuNameEnter();

		if (event.type == sf::Event::Closed) mWindow->close();
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (!hasSelectedCell() && !hasPlayed()) checkCollision(event);
			if (checkTextClick() == 1) return 1;
		}
	}

	changeMenuColor();

	if (!endGame)
	{
		mWindow->clear();
		for (auto& cell : mCells) mWindow->draw(*cell.second.shape);
		for (auto& button : mButton) mWindow->draw(*button);
		for (auto& shape : mShapes) mWindow->draw(*shape);
		if (mEnterName != nullptr) {
			mEnterName->setString("Enter your name: " + mName);
			mWindow->draw(*mEnterName);
		}
		if (hasError) mWindow->draw(*mErrorMessage);
		for (auto& text : mTexts) mWindow->draw(*text);
		for (auto& text : mTextMenu) mWindow->draw(*text);

		mWindow->display();
	}

	return 0;
}

void Window::initCells(std::map<std::pair<int, int>, std::string> cells)
{
	mCells.clear();
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

void Window::resetTurn(bool canPlay)
{
	mHasPlayed = !canPlay;
	mSelectedCell = { -1, -1 };

	//Init texts
	mTexts.clear();

	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();
	text->setFont(mFont);
	text->setCharacterSize(50);
	text->setPosition(mWidth / 5.3, mLength / 9);

	if (canPlay) text->setString("It's your turn to play !");
	else text->setString("Waiting for your opponent...");

	mTexts.push_back(text);
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
				cell.second.player = mPlayerId; //Shape has now been selected
				addPlayerShape(bounds.getPosition(), cell.second.player);
				mSelectedCell = cell.second.pos;
				return;
			}
		}
	}
}

void Window::addPlayerShape(sf::Vector2f position, std::string player)
{
	sf::CircleShape* shape = new sf::CircleShape(75);
	shape->setPosition(position);

	if (player == mPlayerId)
	{
		shape->setFillColor(sf::Color::Green);
	}
	else shape->setFillColor(sf::Color::Red);

	addShape(shape);
}

int Window::checkTextClick()
{
	sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	for (auto& text : mTextMenu)
	{
		if (text == nullptr) continue;

		sf::FloatRect bounds = text->getGlobalBounds();

		if (bounds.contains(mousePosition))
		{
			const std::string& buttonText = text->getString();

			if (buttonText == "Quit")
			{
				mWindow->close();
				return 1;
			}
			else if (buttonText == "Play")
			{
				if (mName.empty()) hasError = true;
				else {
					hasError = false;
					changeScene(WAITING);
					return 0;
				}
			}
			else if (buttonText == "Menu" || buttonText == "Return")
			{
				changeScene(MAIN_MENU);
				return 0;
			}
			else if (buttonText == "Skins")
			{
				changeScene(SKINS);
				return 0;
			}
			else if (buttonText == "Shapes")
			{
				shapesWindow();
				return 0;
			}
			else if (buttonText == "Colors")
			{
				colorsWindow();
				return 0;
			}
		}
	}
	return 0;
}

void Window::changeScene(SceneState newState)
{
	mShapes.clear();
	mTexts.clear();
	mTextMenu.clear();
	mButton.clear();
	mCells.clear();
	mEnterName = nullptr;

	currentScene = newState;

	switch (currentScene)
	{
	case MAIN_MENU:
		textMainMenu();
		shapeMainMenu();
		break;
	case WAITING:
		waitingScreen();
		client->connectClientServer();
		break;
	case SKINS:
		skinsScreen();
		break;
	case GAME:
		break;
	case END_GAME:
		screenEndGame();
		break;
	}
}

void Window::screenEndGame()
{
	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();
	text->setFont(mFont);
	if (mWinner == "None") text->setString("IT'S A TIE !");
	else if (mWinner == mPlayerId) text->setString("YOU WON !");
	else text->setString("YOU LOST...");
	text->setCharacterSize(80);
	text->setPosition(mWidth / 5.3, mLength / 8);
	text->setFillColor(sf::Color(245, 148, 0));
	mTexts.push_back(text);

	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Menu");
	text->setCharacterSize(50);
	text->setPosition(200, 670);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Quit");
	text->setCharacterSize(50);
	text->setPosition(500, 670);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	sf::RectangleShape* button = new sf::RectangleShape();

	//Rectangle "Menu"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(160, 650);
	button->setFillColor(sf::Color(0, 97, 245));
	mButton.push_back(button);

	//Rectangle "Quit"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(450, 650);
	button->setFillColor(sf::Color(0, 97, 245));
	mButton.push_back(button);
}

void Window::textMainMenu()
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

	// Entername text
	mFont.loadFromFile("arial.ttf");
	mEnterName = new sf::Text();
	mEnterName->setFont(mFont);
	mEnterName->setCharacterSize(40);
	mEnterName->setPosition(mWidth / 4.7, mLength / 2.6);
	mEnterName->setFillColor(sf::Color::White);

	//Error enterName text
	mErrorMessage = new sf::Text();
	mErrorMessage->setFont(mFont);
	mErrorMessage->setString("Please, enter a name to play.");
	mErrorMessage->setCharacterSize(30);
	mErrorMessage->setPosition(250, 400);
	mErrorMessage->setFillColor(sf::Color::Red);

	// Play text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Play");
	text->setCharacterSize(50);
	text->setPosition(350, 520);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	// Skins text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Skins");
	text->setCharacterSize(50);
	text->setPosition(190, 670);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	// Quit text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Quit");
	text->setCharacterSize(50);
	text->setPosition(500, 670);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);
}

void Window::shapeMainMenu()
{
	sf::RectangleShape* button = new sf::RectangleShape();

	//Rectangle "Play"
	button->setSize(sf::Vector2f(500.f, 100.f));
	button->setPosition(150, 500);
	button->setFillColor(sf::Color(150, 50, 250));
	mButton.push_back(button);

	//Rectangle "Skins"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(150, 650);
	button->setFillColor(sf::Color(150, 50, 250));
	mButton.push_back(button);

	//Rectangle "Quit"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(450, 650);
	button->setFillColor(sf::Color(150, 50, 250));
	mButton.push_back(button);
}

void Window::menuNameEnter()
{
	if (mEnterName != nullptr && event.type == sf::Event::TextEntered)
	{
		auto processInput = [&](char inputChar)
			{
				if (inputChar == 13) // Touche "Enter"
				{
					if (!mName.empty())
					{
						hasError = false;
						changeScene(WAITING);
					}
				}
				else if (inputChar == 8) // Touche "Backspace"
				{
					if (!mName.empty())
					{
						mName.pop_back();
					}
				}
				else if (inputChar < 128 && mName.length() < maxNameLength)
				{
					mName += static_cast<char>(inputChar);
				}
			};
		processInput(event.text.unicode);
	}
}

void Window::changeMenuColor()
{
	//Récupère la position de la souris sur la fenêtre
	sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(*mWindow));

	//Boucle sur notre texte
	for (auto& text : mTextMenu)
	{
		auto gb = text->getGlobalBounds();
		if (gb.contains(pos)) text->setFillColor(sf::Color(222, 31, 63));
		else text->setFillColor(sf::Color::White);
	}
}

void Window::waitingScreen()
{
	//Waiting text
	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();
	text->setFont(mFont);
	text->setString("Waiting for players...");
	text->setCharacterSize(50);
	text->setPosition(mWidth / 4, mLength / 3);
	text->setFillColor(sf::Color(255, 255, 255));
	mTexts.push_back(text);
}

void Window::skinsScreen()
{
	shapesWindow();
	colorsWindow();

	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();

	//Return Text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Return");
	text->setCharacterSize(50);
	text->setPosition(625, 20);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	//Shapes
	sf::RectangleShape* button = new sf::RectangleShape();

	//Button "Return"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(600, 0);
	button->setFillColor(sf::Color(150, 50, 50));
	mButton.push_back(button);
}

void Window::shapesWindow()
{
	//Texts
	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();

	//Shapes Text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Shapes");
	text->setCharacterSize(50);
	text->setPosition(10, 20);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	sf::RectangleShape* button = new sf::RectangleShape();

	//Button "Shapes"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(0, 0);
	button->setFillColor(sf::Color(150, 50, 250));
	mButton.push_back(button);

	//Bg "Shapes"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(800.f, 700.f));
	button->setPosition(0, 100);
	button->setFillColor(sf::Color(150, 50, 250));
	mButton.push_back(button);

	// Slot shapes
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			sf::RectangleShape* button = new sf::RectangleShape();
			button->setSize(sf::Vector2f(cote, cote));
			button->setPosition(62.5 + j * 250, 150 + i * 225);
			button->setFillColor(sf::Color(211, 211, 211));
			mButton.push_back(button);
			std::pair<int, int> p = { i, j };
			mSkinsCells[p] = button;
		}
	}

	//Cercle
	sf::CircleShape* circle = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	circle->setPosition(mSkinsCells[{0, 0}]->getPosition());  // Définit la position du cercle
	circle->setFillColor(sf::Color::Black);  // Définit la couleur de remplissage
	skins[{0, 0}] = circle;
	mShapes.push_back(circle);  // Ajoute le cercle à votre vecteur de formes

	//Triangle
	sf::ConvexShape* triangle = new sf::ConvexShape(3);  // Crée un triangle avec 3 points
	float sideLength = 100.0f;  // Longueur des côtés du triangle
	float height = (sqrt(3.0f) / 2.0f) * sideLength;  // Hauteur du triangle
	triangle->setPoint(0, sf::Vector2f(0.5f * cote, 0.5f * cote + height));  // Centre en x: 0.5f * cote, Centre en y: 0.5f * cote + height
	triangle->setPoint(1, sf::Vector2f(0.5f * cote + sideLength / 2.0f, 0.5f * cote));  // Centre en x: 0.5f * cote + sideLength / 2.0f, Centre en y: 0.5f * cote
	triangle->setPoint(2, sf::Vector2f(0.5f * cote + sideLength, 0.5f * cote + height));  // Centre en x: 0.5f * cote + sideLength, Centre en y: 0.5f * cote + height
	triangle->setPosition(mSkinsCells[{0, 1}]->getPosition());  // Définit la position du cercle
	triangle->setFillColor(sf::Color::Black);  // Définit la couleur de remplissage
	skins[{0, 1}] = triangle;
	mShapes.push_back(triangle);  // Ajoute le triangle à votre vecteur de formes

}


void Window::colorsWindow()
{
	mFont.loadFromFile("arial.ttf");
	sf::Text* text = new sf::Text();

	//Colors Text
	text = new sf::Text();
	text->setFont(mFont);
	text->setString("Colors");
	text->setCharacterSize(50);
	text->setPosition(220, 20);
	text->setFillColor(sf::Color::White);
	mTextMenu.push_back(text);

	//Shapes
	sf::RectangleShape* button = new sf::RectangleShape();

	//Button "Colors"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(200.f, 100.f));
	button->setPosition(200, 0);
	button->setFillColor(sf::Color(0, 97, 245));
	mButton.push_back(button);

	//Bg "Colors"
	button = new sf::RectangleShape();
	button->setSize(sf::Vector2f(800.f, 700.f));
	button->setPosition(0, 100);
	button->setFillColor(sf::Color(0, 97, 245));
	mButton.push_back(button);

	// Slot colors
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			sf::RectangleShape* button = new sf::RectangleShape();
			button->setSize(sf::Vector2f(cote, cote));
			button->setPosition(62.5 + j * 250, 150 + i * 225);
			button->setFillColor(sf::Color(211, 211, 211));
			mButton.push_back(button);
			std::pair<int, int> p = { i, j };
			mSkinsCells[p] = button;
		}
	}

	//Pink
	sf::CircleShape* Pink = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Pink->setPosition(mSkinsCells[{0, 0}]->getPosition());  // Définit la position du cercle
	Pink->setFillColor(sf::Color(255, 182, 193));  // Définit la couleur de remplissage
	skins[{0, 0}] = Pink;
	mShapes.push_back(Pink);  // Ajoute le cercle à votre vecteur de formes

	//Mint
	sf::CircleShape* Mint = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Mint->setPosition(mSkinsCells[{0, 1}]->getPosition());  // Définit la position du cercle
	Mint->setFillColor(sf::Color(152, 255, 152));  // Définit la couleur de remplissage
	skins[{0, 1}] = Mint;
	mShapes.push_back(Mint);  // Ajoute le cercle à votre vecteur de formes

	//Magenta
	sf::CircleShape* Magenta = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Magenta->setPosition(mSkinsCells[{0, 2}]->getPosition());  // Définit la position du cercle
	Magenta->setFillColor(sf::Color::Magenta);  // Définit la couleur de remplissage
	skins[{0, 2}] = Magenta;
	mShapes.push_back(Magenta);  // Ajoute le cercle à votre vecteur de formes

	//Yellow
	sf::CircleShape* Yellow = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Yellow->setPosition(mSkinsCells[{1, 0}]->getPosition());  // Définit la position du cercle
	Yellow->setFillColor(sf::Color::Yellow);  // Définit la couleur de remplissage
	skins[{1, 0}] = Yellow;
	mShapes.push_back(Yellow);  // Ajoute le cercle à votre vecteur de formes

	//Orange
	sf::CircleShape* Orange = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Orange->setPosition(mSkinsCells[{1, 1}]->getPosition());  // Définit la position du cercle
	Orange->setFillColor(sf::Color(255, 165, 79));  // Définit la couleur de remplissage
	skins[{1, 1}] = Orange;
	mShapes.push_back(Orange);  // Ajoute le cercle à votre vecteur de formes

	//Marron
	sf::CircleShape* Marron = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Marron->setPosition(mSkinsCells[{1, 2}]->getPosition());  // Définit la position du cercle
	Marron->setFillColor(sf::Color(139, 69, 19));  // Définit la couleur de remplissage
	skins[{1, 2}] = Marron;
	mShapes.push_back(Marron);  // Ajoute le cercle à votre vecteur de formes

	//Red
	sf::CircleShape* Red = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Red->setPosition(mSkinsCells[{2, 0}]->getPosition());  // Définit la position du cercle
	Red->setFillColor(sf::Color::Red);  // Définit la couleur de remplissage
	skins[{2, 0}] = Red;
	mShapes.push_back(Red);  // Ajoute le cercle à votre vecteur de formes

	//Green
	sf::CircleShape* Green = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	Green->setPosition(mSkinsCells[{2, 1}]->getPosition());  // Définit la position du cercle
	Green->setFillColor(sf::Color::Green);  // Définit la couleur de remplissage
	skins[{2, 1}] = Green;
	mShapes.push_back(Green);  // Ajoute le cercle à votre vecteur de formes

	//Ligt Blue
	sf::CircleShape* LightBlue = new sf::CircleShape(75.f);  // Crée un cercle de rayon 50
	LightBlue->setPosition(mSkinsCells[{2, 2}]->getPosition());  // Définit la position du cercle
	LightBlue->setFillColor(sf::Color(135, 206, 250));  // Définit la couleur de remplissage
	skins[{2, 2}] = LightBlue;
	mShapes.push_back(LightBlue);  // Ajoute le cercle à votre vecteur de formes
}
#include "Game.h"

// Initialisation
void Game::initVariables()
{
	this->window = nullptr;
	this->client = new Client();
	this->textureCross.loadFromFile("Textures/cross.png");//il faudrait push avec ton png à toi crossTransparentFont.png");
	this->font.loadFromFile("Font/arial.ttf");

	initGrid();
	initText();
	
	threadAnalyseDataReceived = thread(&Game::analyseDataReceived, this);

	//initCrossSymbol();
	//initCircleSymbol();
}

void Game::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 1280;
	this->window = new sf::RenderWindow(this->videoMode, "Morpion",sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);
}

//void Game::initCircleSymbol()
//{
//	this->circleSymbol.setRadius(110.f);
//	this->circleSymbol.setFillColor(Color::White);
//	this->circleSymbol.setPosition(Vector2f(42.5f, 42.5f));
//	this->circleSymbol.setOutlineThickness(-20.f);
//	this->circleSymbol.setOutlineColor(Color::Black);
//}
//

Sprite Game::initCrossSymbol(Sprite crossSymbol, Vector2f pos)
{
	crossSymbol.setPosition(pos);
	crossSymbol.setScale(Vector2f(0.45f, 0.45f));

	return crossSymbol;
}

void Game::initGrid()
{
	this->square.setSize(Vector2f(760.f, 760.f));
	this->square.setFillColor(Color::Transparent);
	this->square.setPosition(Vector2f(25.f, 25.f));
	this->square.setOutlineThickness(-10.f);
	this->square.setOutlineColor(Color::Black);

	// Ligne du haut du morpion
	this->lineUp.setSize(Vector2f(750.f, 10.f));
	this->lineUp.setPosition(Vector2f(25.f, 275.f));
	this->lineUp.setFillColor(Color::Black);

	// Ligne du bas du morpion
	this->lineDown.setSize(Vector2f(750.f, 10.f));
	this->lineDown.setPosition(Vector2f(25.f, 525.f));
	this->lineDown.setFillColor(Color::Black);

	// Ligne de gauche du morpion
	this->lineLeft.setSize(Vector2f(10.f, 750.f));
	this->lineLeft.setPosition(Vector2f(275.f, 25.f));
	this->lineLeft.setFillColor(Color::Black);

	// Ligne de droite du morpion
	this->lineRight.setSize(Vector2f(10.f, 750.f));
	this->lineRight.setPosition(Vector2f(525.f, 25.f));
	this->lineRight.setFillColor(Color::Black);
}

void Game::initText()
{
	this->text.setFont(this->font);
	this->text.setFillColor(Color::Cyan);
	this->text.setCharacterSize(40);
	this->text.setPosition(Vector2f(800.f, 25.f));
}


// Constructors and destructors
Game::Game()
{
	initVariables();
	initWindow();
}

Game::~Game()
{
	delete this->window;
}

// Accessors
const bool Game::running() const
{
	return this->window->isOpen();

}

// Functions
void Game::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		if (this->event.type == Event::Closed) {
			this->closeConnection();
			this->window->close();

		}
	}
}


void Game::updateMousePos()
{
	this->mousePos = Mouse::getPosition(*this->window);
}

void Game::checkMouseClick()
{

	if (Mouse::isButtonPressed(Mouse::Left) && (leftClickIsPressed == false)) {
		leftClickIsPressed = true;
		this->onMouseClick();
	}
	else {
		if (not Mouse::isButtonPressed(Mouse::Left))
			leftClickIsPressed = false;
	}
}

void Game::checkWin()
{
	if (listPosOccupied.size() == 9)
		this->text.setString("Match nul");
}

void Game::update()
{
	pollEvents();
	updateMousePos();
	checkMouseClick();
	checkWin();
}

void Game::render()
{
	// Clear old frame
	this->window->clear(sf::Color::White);

	// Draw game objects
	this->drawElements();
	this->drawGrid();
	this->window->draw(this->text);


	// Tell that we have finished to draw the new frame
	this->window->display();
}

void Game::drawGrid()
{
	this->window->draw(this->square);
	this->window->draw(this->lineUp);
	this->window->draw(this->lineDown);
	this->window->draw(this->lineLeft);
	this->window->draw(this->lineRight);
}

void Game::drawElements()
{
	for (int i = 0; i < this->listCrossElement.size(); i++) {
		this->window->draw(this->listCrossElement[i]);
	}

	for (int j = 0; j < this->listCircleElement.size(); j++) {
		this->window->draw(this->listCircleElement[j].getCircleSymbol());
	}
}

void Game::onMouseClick()
{
	if ((25 < this->mousePos.x) && (this->mousePos.x < 750) && (25 < this->mousePos.y) && (this->mousePos.y < 750)) {
		int posGrid = (this->mousePos.x - 25) / 250 + ((this->mousePos.y - 25) / 250) * 3;
		this->createSymbol(posGrid);
	}
	else {
		cout << "En dehors de la grille" << endl;
	}
}

void Game::createSymbol(int posGrid)
{
	if ((find(listPosOccupied.begin(), listPosOccupied.end(), posGrid) == listPosOccupied.end()) || (listPosOccupied.size() == 0) || ((listPosOccupied.size() == 1) && (listPosOccupied[0] != posGrid))) {
		listPosOccupied.push_back(posGrid);
		if (circleToDraw) {
			Circle newCircle(posGrid);
			listCircleElement.push_back(newCircle);
			// Envoi d'un coup
		}
		else {
			CrossInfo newCrossInfo(posGrid);
			Sprite newCrossSymbol(this->textureCross);
			newCrossSymbol = initCrossSymbol(newCrossSymbol, newCrossInfo.getCrossSymbolPos());

			listCrossElement.push_back(newCrossSymbol);
		}
		circleToDraw = !circleToDraw;
	}
	else {
		cout << "Case deja occupee par un symbole" << endl;
	}
}

void Game::closeConnection()
{
	this->socketIsOpened = false;
	// A la place:
	// il faut envoyer un packet avec l'action CLOSE_CONNECTION
}

void Game::analyseMessage(string lastMessage)
{

}

void Game::analyseDataReceived()
{
	while (this->socketIsOpened) {
		if (this->client->getMessagesReceived().size() > 0) {
			this->client->mutexReceiving.lock();

			string lastMessage = this->client->getMessagesReceived().front();
			this->analyseMessage(lastMessage);

			queue<string> newListMessagesReceived;

			newListMessagesReceived = this->client->getMessagesReceived();

			newListMessagesReceived.pop();

			this->client->setMessagesReceived(newListMessagesReceived);

			this->client->mutexReceiving.unlock();
		}



	}
}





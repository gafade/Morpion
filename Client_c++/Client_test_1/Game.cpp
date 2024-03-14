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
		//this->onMouseClick();
		this->newonMouseClick();
	}
	else {
		if (not Mouse::isButtonPressed(Mouse::Left))
			leftClickIsPressed = false;
	}
}

void Game::checkWin()
{
	if (listCircleOccupied.size() == 9) {//stocke valeurs de -9 à -1 et de 1 à 9


		int circles[9] = { 0,0,0,0,0,0,0,0,0 };
		int crosses[9] = { 0,0,0,0,0,0,0,0,0 };
		for (int i = 0;i < listCircleOccupied.size();i++) {
			//positive if cross, negative if circle
			if (listCircleOccupied[i]<0) {
				circles[i] = -listCircleOccupied[i];
			}
			else {
				crosses[i] = listCircleOccupied[i];
			}
		}
		//Solutions:
		/*
		-lignes : 012,345,678 : step of 1
		-colonnes: 036,147,258: step of 3 
		-diagonales: 048,246 : step of 4,step of 2 
		*/

		bool moves[9] = { 0,0,0 ,0,0,0 ,0,0,0 };

		for (int pos = 1;pos <= 9;pos++) {
			for (int i = 0;i < 9;i++) {
				if (circles[i] == pos) {
					moves[pos-1] = 1;
					break;
				}
			}
		}
 		//lignes
		if (moves[0] == moves[1] and moves[1] == moves[2]) { this->WinMessage(moves[0]); }
		else if (moves[3] == moves[4] and moves[4] == moves[5]) { this->WinMessage(moves[3]); }
		else if (moves[6] == moves[7] and moves[7] == moves[8]) { this->WinMessage(moves[6]); }
		//colonnes
		else if (moves[0] == moves[3] and moves[3] == moves[6]) { this->WinMessage(moves[0]); }
		else if (moves[1] == moves[4] and moves[4] == moves[7]) { this->WinMessage(moves[1]); }
		else if (moves[2] == moves[5] and moves[5] == moves[8]) { this->WinMessage(moves[2]); }
		//diagonales
		else if (moves[0] == moves[4] and moves[4] == moves[8]) { this->WinMessage(moves[0]); }
		else if (moves[2] == moves[4] and moves[4] == moves[6]) { this->WinMessage(moves[2]); }
		//other
		else {
			this->text.setString("Match nul");
		}
	}
}

void Game::WinMessage(bool who) {
	if (who == 1) {
		this->text.setString("Cercle a gagné");
	}
	else {
		this->text.setString("Croix a gagné");
	}
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
// test
//instead of filling IF there is a mouse click, we will first send the click to server 
//AND THEN wait for the server's answer
//server has a variable to decide which turn it is to play :
//every correct click is sent to the server and only the correct one warrants a "accepted move response"
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

void Game::newonMouseClick() {

	if ((25 < this->mousePos.x) && (this->mousePos.x < 750) && (25 < this->mousePos.y) && (this->mousePos.y < 750)) {
		int posGrid = (this->mousePos.x - 25) / 250 + ((this->mousePos.y - 25) / 250) * 3;
		//client->sendMessage();
		// : is the delimiter of : type:move:numPlayer:pseudo:opponent
		string message = "2:"+to_string(posGrid)+":0:nAn:NaN";//2 for indicating a move
		cout << "Played : "<<to_string(posGrid) << endl;
		client->addMessageToQueue(message);//le Uint32 le transforem en emoji??
	}
	else {
		cout << "En dehors de la grille" << endl;
	}
}

void Game::createSymbol(int posGrid)
{
	if ((find(listPosOccupied.begin(), listPosOccupied.end(), posGrid) == listPosOccupied.end()) || (listPosOccupied.size() == 0) || ((listPosOccupied.size() == 1) && (listPosOccupied[0] != posGrid))) {
		listPosOccupied.push_back(posGrid);//positive if cross, negative if circle
		listCircleOccupied.push_back((int)(posGrid+1) * pow(-1, circleToDraw));//de -9 à -1 et de 1 à 9
		if (circleToDraw) {
			Circle newCircle(posGrid);
			listCircleElement.push_back(newCircle);
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
	//what the server can say in utf-8 :

	//Welcome
	//Currently searching for game
	//Game start
	//Circle [pos]
	//Cross [pos]
	//Victory
	//Lose 
	//Disconnect
	//Gooodbye
	 
	
	//NOn packet stuff is already just cout-ed
	//here is the formating of packets server-side
	//return "{}:{}:{}:".format(self.action.value, self.move, self.numPlayer) + self.pseudo + ":" \
            + self.pseudoOpponent
	string component[7] = { "None","None","None","None","None","None","None" };
	int i = 0;
	int first = 0;
	int last = lastMessage.find(":",first);
	while (lastMessage.find(":", first) != -1 and i<7) {
		if (first == 0) {//this is the first substring
			component[i] = (lastMessage.substr(first, last - first));
		}
		else if (last == -1) {//this is the last substring
			component[i] = lastMessage.substr(first+1);
		}
		else {
			component[i] = (lastMessage.substr(first+1, last - first-1));
		}
		cout << "Step by Step : " << component[i] << endl;
		i++;
		first = last;
		last = lastMessage.find(":",first+1);
		
	}

	if (component[0] == "1") {//LAUNCH GAME PACKET
		cout << "Game found against : " << component[4] << endl << endl;
	}
	else if(component[0]=="2") {
		cout << "Someone played : " << component[1] << endl;
		this->createSymbol( stoi(component[1]));
	}
	else {
		cout << "NON RECOGNIZED PACKET";
	}
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





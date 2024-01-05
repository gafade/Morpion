#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Client.h"
#include <vector>
#include "CrossInfo.h"
#include "Circle.h"


using namespace std;
using namespace sf;

class Game
{
private:

	// Variables
	RenderWindow* window;
	Event event;
	VideoMode videoMode;

	RectangleShape square;
	//CircleShape circleSymbol;
	RectangleShape lineUp;
	RectangleShape lineDown;
	RectangleShape lineLeft;
	RectangleShape lineRight;
	//Sprite crossSymbol;
	Texture textureCross;

	Vector2i mousePos;

	vector<Sprite> listCrossElement;
	vector<Circle> listCircleElement;
	vector<int> listPosOccupied;

	bool circleToDraw = true;
	bool leftClickIsPressed;

	Font font;
	Text text;

	Client* client = nullptr;


	// Functions
	void initVariables();
	void initWindow();
	void initGrid();
	void initText();
	Sprite initCrossSymbol(Sprite crossSymbol, Vector2f pos);
	//void initCircleSymbol();
	
	void drawGrid();
	void drawElements();
	void checkMouseClick();
	void checkWin();
	void onMouseClick();
	void createSymbol(int posGrid);
	void closeConnection();



public:

	Game();
	virtual ~Game();


	// Accessors
	const bool running() const;

	// Functions
	void pollEvents();
	void updateMousePos();
	void update();
	void render();





};


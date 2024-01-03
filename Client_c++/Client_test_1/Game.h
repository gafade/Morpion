#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Client.h"


using namespace std;
using namespace sf;

class Game
{
private:

	// Variables
	sf::RenderWindow* window;
	sf::Event event;
	sf::VideoMode videoMode;

	Client* client = nullptr;


	// Functions
	void initVariables();
	void initWindow();



public:

	Game();
	virtual ~Game();


	// Accessors
	const bool running() const;

	// Functions
	void pollEvents();
	void update();
	void render();




};


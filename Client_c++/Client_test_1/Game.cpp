#include "Game.h"

// Initialisation
void Game::initVariables()
{
	this->window = nullptr;
	this->client = new Client();
}

void Game::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 1280;
	this->window = new sf::RenderWindow(this->videoMode, "Morpion",sf::Style::Titlebar | sf::Style::Close);
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
		if (this->event.type == sf::Event::Closed)
			this->window->close();
	}
}

void Game::update()
{
	pollEvents();
}

void Game::render()
{
	// Clear old frame
	this->window->clear(sf::Color::Blue);

	// Draw game objects

	// Tell that we have finished to draw the new frame
	this->window->display();

}

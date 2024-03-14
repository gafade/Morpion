#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


using namespace sf;
using namespace std;


class CrossInfo
{
private:
	Vector2f pos;
	int posGrid;

	// Functions
	void calculateCrossPos();

public:

	CrossInfo(int posGrid);

	virtual ~CrossInfo();

	// Getter
	Vector2f getCrossSymbolPos() {
		return this->pos;
	}
	int getPosGrid() {
		return this->posGrid;
	}
};


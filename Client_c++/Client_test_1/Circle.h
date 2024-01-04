#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


using namespace sf;
using namespace std;

class Circle
{
private:
	Vector2f pos;
	int posGrid;

	CircleShape circleSymbol;


	// Functions
	void calculateCirclePos();
	void initCircleSymbol();

public:

	Circle(int posGrid);

	virtual ~Circle();

	// Getter
	CircleShape getCircleSymbol() {
		return this->circleSymbol;
	}
};


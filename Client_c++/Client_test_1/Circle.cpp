#include "Circle.h"

Circle::Circle(int posGrid)
{
	// Va de 0 (en haut à gauche) à 8 (en bas à droite)
	this->posGrid = posGrid;

	calculateCirclePos();
	initCircleSymbol();
}

Circle::~Circle()
{
}

void Circle::calculateCirclePos()
{
	this->pos.x = 42.5f + (float)((this->posGrid % 3) * (250.f));
	this->pos.y = 42.5f + (float)((this->posGrid / 3) * (250.f));
}

void Circle::initCircleSymbol()
{
	this->circleSymbol.setRadius(110.f);
	this->circleSymbol.setFillColor(Color::Transparent);
	this->circleSymbol.setOutlineThickness(-20.f);
	this->circleSymbol.setOutlineColor(Color::Black);
	this->circleSymbol.setPosition(this->pos);
}

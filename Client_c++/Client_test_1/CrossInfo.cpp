#include "CrossInfo.h"

CrossInfo::CrossInfo(int posGrid)
{
	// Va de 0 (en haut � gauche) � 8 (en bas � droite)
	this->posGrid = posGrid;

	calculateCrossPos();
}

CrossInfo::~CrossInfo()
{
}

void CrossInfo::calculateCrossPos()
{
	this->pos.x = 35.f + (float)((this->posGrid % 3) * (250.f));
	this->pos.y = 35.f + (float)((this->posGrid / 3) * (250.f));
}


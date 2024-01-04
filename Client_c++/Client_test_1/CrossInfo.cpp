#include "CrossInfo.h"

CrossInfo::CrossInfo(int posGrid)
{
	// Va de 0 (en haut à gauche) à 8 (en bas à droite)
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


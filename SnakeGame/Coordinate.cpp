#include "Coordinate.h"

Coordinate::Coordinate(int y, int x)
{
	_x = x;
	_y = y;
}

int Coordinate::getX()
{
	return _x;
}

int Coordinate::getY()
{
	return _y;
}

void Coordinate::setX(int x)
{
	_x = x;
}

void Coordinate::setY(int y)
{
	_y = y;
}

bool Coordinate::operator!=(Coordinate& rhs)
{
	return this->getX() != rhs.getX() || this->getY() != rhs.getY();
}

bool Coordinate::operator==(Coordinate& rhs)
{
	return this->getX() == rhs.getX() && this->getY() == rhs.getY();
}

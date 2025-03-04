#pragma once
class Coordinate
{
public:
	Coordinate(int y = 0, int x = 0);
	int getX();
	int getY();
	void setX(int x);
	void setY(int y);
	bool operator!=(Coordinate& rhs);
	bool operator==(Coordinate& rhs);
private:
	int _x;
	int _y;
};
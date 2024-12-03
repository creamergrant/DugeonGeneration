#pragma once

#include "GameDev2D.h"

struct Point
{
	GameDev2D::Vector2 p;
	int id = 0;

	Point(){}
	Point(GameDev2D::Vector2 p, int id);
	Point(int x, int y, int z);

	bool operator==(const Point& o) const;
};
#pragma once

#include "Point.h"

struct Triangle
{
	Point v1, v2, v3;

	Triangle(const Point& p1, const Point& p2, const Point& p3);

	bool ContainsPoint(const Point& o) const;
};
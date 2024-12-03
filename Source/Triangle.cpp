#include "Triangle.h"

Triangle::Triangle(const Point& p1, const Point& p2, const Point& p3) :
	v1(p1),
	v2(p2),
	v3(p3)
{
}

bool Triangle::ContainsPoint(const Point& o) const
{
	//funky shit to see if point is in a triangle
	float ax = v1.p.x - o.p.x;
	float ay = v1.p.y - o.p.y;
	float bx = v2.p.x - o.p.x;
	float by = v2.p.y - o.p.y;
	float cx = v3.p.x - o.p.x;
	float cy = v3.p.y - o.p.y;

	float detAB = ax * by - ay * bx;
	float detBC = bx * cy - by * cx;
	float detCA = cx * ay - cy * ax;

	float a2 = ax * ax + ay * ay;
	float b2 = bx * bx + by * by;
	float c2 = cx * cx + cy * cy;

	float det = (a2 * detBC) + (b2 * detCA) + (c2 * detAB);

	return det > 0;
}

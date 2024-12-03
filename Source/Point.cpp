#include "Point.h"

Point::Point(GameDev2D::Vector2 p, int id) :
	p(p),
	id(id)
{
}

Point::Point(int x, int y, int z) :
	p((float)x, (float)y),
	id(z)
{
}

bool Point::operator==(const Point& o) const
{
	return id == o.id;
}

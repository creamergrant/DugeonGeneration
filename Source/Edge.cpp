#include "Edge.h"

Edge::Edge(const Point& p1, const Point& p2) :
	v1(p1),
	v2(p2)
{
}

Edge::Edge(const GameDev2D::Vector2& p1, const GameDev2D::Vector2& p2)
{
	v1 = Point(p1, 0);
	v2 = Point(p2, 0);
}

bool Edge::operator<(const Edge& o) const
{
	if (v1.p.x == o.v1.p.x && v1.p.y == o.v1.p.y) 
	{
		return (v2.p.x < o.v2.p.x || (v2.p.x == o.v2.p.x && v2.p.y < o.v2.p.y));
	}
	return (v1.p.x < o.v1.p.x || (v1.p.x == o.v1.p.x && v1.p.y < o.v1.p.y));
}

void Edge::Render(GameDev2D::BatchRenderer& renderer)
{
	renderer.RenderLine(v1.p, v2.p, color, 6);
}

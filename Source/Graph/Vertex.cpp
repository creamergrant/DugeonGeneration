#include "Vertex.h"

Vertex::Vertex(const Point& point, float dist) :
	m_point(point),
	m_id(point.id),
	m_dist(dist)
{
}

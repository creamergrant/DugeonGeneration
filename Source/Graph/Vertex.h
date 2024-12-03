#pragma once

#include <list>
#include <utility>
#include "../Point.h"

struct Vertex
{
	std::list<std::pair<float, Vertex&>> m_edges;

	float m_dist = 0;

	int m_id = 0;

	Point m_point;

	Vertex(const Point& point, float dist);
};
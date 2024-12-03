#pragma once

#include "Point.h"

struct Edge
{
	Point v1, v2;

	GameDev2D::Color color = GameDev2D::ColorList::LightGreen;

	Edge(const Point& p1, const Point& p2);

	Edge(const GameDev2D::Vector2& p1, const GameDev2D::Vector2& p2);

	bool operator<(const Edge& o) const;

	void Render(GameDev2D::BatchRenderer& renderer);

	void SetColor(GameDev2D::Color c) { color = c; }
};
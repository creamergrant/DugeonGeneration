#pragma once

#include "Vector2.h"

namespace GameDev2D
{
	struct LineSegment
	{
		LineSegment(const Vector2& start, const Vector2& end);
		LineSegment(float startX, float startY, float endX, float endY);
		LineSegment();

		float Length() const;
		Vector2 Normal() const;
		Vector2 Midpoint() const;
		Vector2 Delta() const;

		bool DoesIntersect(const LineSegment& lineSegment, Vector2& intersectionPoint);

		Vector2 start;
		Vector2 end;
	};
}
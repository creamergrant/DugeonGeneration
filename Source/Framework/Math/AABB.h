#pragma once

#include "Vector2.h"
#include "LineSegment.h"

namespace GameDev2D
{
	struct AABB
	{
		AABB(const Vector2& lower, const Vector2& upper);
		AABB();
		~AABB() = default;

		Vector2 Center() const;
		Vector2 GetDimensions() const;
		Vector2 GetHalfDimensions() const;

		bool DoesIntersectWithCircle(const LineSegment& circlePath, float circleRadius, Vector2& intersectionPoint, Vector2& collisionNormal);

		bool Intersects(const AABB& other) const;
		bool Contains(const AABB& other) const;

		bool ResolveCollision(AABB& other);

		Vector2 lower;
		Vector2 upper;

		float RoundM(float n, float m);
	};
}
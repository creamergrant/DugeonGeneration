#include "LineSegment.h"


namespace GameDev2D
{
	LineSegment::LineSegment(const Vector2& start, const Vector2& end) :
		start(start),
		end(end)
	{}

	LineSegment::LineSegment(float startX, float startY, float endX, float endY) :
		start(startX, startY),
		end(endX, endY)
	{}

	LineSegment::LineSegment() :
		start(0.0f, 0.0f),
		end(0.0f, 0.0f)
	{}

	float LineSegment::Length() const
	{
		float distance = start.Distance(end);
		return distance;
	}

	Vector2 LineSegment::Normal() const
	{
		Vector2 normal = start - end;
		return normal.PerpendicularCounterClockwise().Normalized();
	}

	Vector2 LineSegment::Midpoint() const
	{
		return Vector2((start + end) * 0.5f);
	}

	Vector2 LineSegment::Delta() const
	{
		return Vector2(end.x - start.x, end.y - start.y);
	}

	bool LineSegment::DoesIntersect(const LineSegment& lineSegment, Vector2& intersectionPoint)
	{
		Vector2 deltaA = Delta();
		Vector2 deltaB = lineSegment.Delta();

		float s = (-deltaA.y * (start.x - lineSegment.start.x) + deltaA.x * (start.y - lineSegment.start.y)) / (-deltaB.x * deltaA.y + deltaA.x * deltaB.y);
		float t = (deltaB.x * (start.y - lineSegment.start.y) - deltaB.y * (start.x - lineSegment.start.x)) / (-deltaB.x * deltaA.y + deltaA.x * deltaB.y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			//Collision detected
			intersectionPoint.x = start.x + (t * deltaA.x);
			intersectionPoint.y = start.y + (t * deltaA.y);
			return true;
		}

		//No collision
		return false; 
	}
}
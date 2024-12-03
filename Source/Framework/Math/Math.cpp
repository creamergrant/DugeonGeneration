#include "Math.h"
#include <math.h>
#include <stdlib.h>


namespace GameDev2D
{
#ifdef RANDOM_SEED
    Random Math::s_Random(RANDOM_SEED);
#else
    Random Math::s_Random = Random();
#endif

    const int Math::BottomLeftCorner = 0;
    const int Math::BottomRightCorner = 1;
    const int Math::TopRightCorner = 2;
    const int Math::TopLeftCorner = 3;

    void Math::CalculateCorners(const Matrix& inTransform, const Vector2& inAnchor, const Vector2& inSize, Vector2 outCorners[4])
    {
        Vector2 p(inTransform.GetTranslation());
        float r = inTransform.GetRadians();
        float c = cosf(r);
        float s = sinf(r);
        float ax = inAnchor.x;
        float ay = inAnchor.y;

        float offsetX = inSize.x * 0.5f;
        float offsetY = inSize.y * 0.5f;
        float anchorX = inSize.x * (ax - 0.5f);
        float anchorY = inSize.y * (ay - 0.5f);

        //Rotated top left 
        float tlx = p.x - (offsetX * c) - (offsetY * s) - (anchorX * c) + (anchorY * s);
        float tly = p.y - (offsetX * s) + (offsetY * c) - (anchorX * s) - (anchorY * c);

        //Rotated bottom left
        outCorners[BottomLeftCorner].x = p.x - (offsetX * c) + (offsetY * s) - (anchorX * c) + (anchorY * s);
        outCorners[BottomLeftCorner].y = p.y - (offsetX * s) - (offsetY * c) - (anchorX * s) - (anchorY * c);

        //Rotated bottom right 
        outCorners[BottomRightCorner].x = p.x + (offsetX * c) + (offsetY * s) - (anchorX * c) + (anchorY * s);
        outCorners[BottomRightCorner].y = p.y + (offsetX * s) - (offsetY * c) - (anchorX * s) - (anchorY * c);

        //Rotated top right 
        outCorners[TopRightCorner].x = p.x + (offsetX * c) - (offsetY * s) - (anchorX * c) + (anchorY * s);
        outCorners[TopRightCorner].y = p.y + (offsetX * s) + (offsetY * c) - (anchorX * s) - (anchorY * c);

        //Rotated top left 
        outCorners[TopLeftCorner].x = p.x - (offsetX * c) - (offsetY * s) - (anchorX * c) + (anchorY * s);
        outCorners[TopLeftCorner].y = p.y - (offsetX * s) + (offsetY * c) - (anchorX * s) - (anchorY * c);
    }

    Vector2 Math::CalculateClosestPointOnLine(const LineSegment& lineSegment, const Vector2& point)
    {
        //Calculate the closest point of the line
        Vector2 lineStart = lineSegment.start;
        Vector2 lineEnd = lineSegment.end;

        //Calculate the circle vector        
        Vector2 circleVector = point - lineStart;

        //Calculate the line segment vector        
        Vector2 lineVector = lineEnd - lineStart;

        //Normalize the line segment vector        
        Vector2 normalizedVector = lineVector.Normalized();

        //Calculate the dot product between the circle vector and the normalized line segment vector       
        float magnitude = normalizedVector.DotProduct(circleVector);

        //Calculate the projection using the result of the dot product and multiply it by the normalized line segment        
        Vector2 projection = normalizedVector * magnitude;

        //Calculate the closest point on the line segment, by adding the project vector to the line start vector        
        Vector2 closestPoint = lineStart + projection;

        //Ensure the closest point is actually on the line
        closestPoint = Math::Clamp(closestPoint, lineStart < lineEnd ? lineStart : lineEnd, lineStart < lineEnd ? lineEnd : lineStart);
        return closestPoint;
    }

    Vector2 Math::RotatePoint(const Vector2& point, const Vector2& targetPoint, float targetRadians)
    {
        float c = cosf(targetRadians);
        float s = sinf(targetRadians);

        Vector2 rotatedPoint;
        rotatedPoint.x = c * (point.x - targetPoint.x) - s * (point.y - targetPoint.y) + targetPoint.x;
        rotatedPoint.y = s * (point.x - targetPoint.x) + c * (point.y - targetPoint.y) + targetPoint.y;

        return rotatedPoint;
    }

    bool Math::IsPointOnLineSegment(const LineSegment& lineSegment, const Vector2& point, float range)
    {
        Vector2 closestPoint = CalculateClosestPointOnLine(lineSegment, point);
        bool isCloseEnough = IsClose(closestPoint.x, point.x, range) && IsClose(closestPoint.y, point.y, range);
        return isCloseEnough;
    }

    bool Math::IsPointInsideCircle(const Vector2& circleCenter, float circleRadius, const Vector2& point)
    {
        float distanceSquared = circleCenter.DistanceSquared(point);
        return distanceSquared < circleRadius * circleRadius;
    }

    bool Math::IsPointInsideRectangle(const Vector2& rectangleCenter, const Vector2& rectangleSize, const Vector2& point)
    {
        float rectLeftEdge = rectangleCenter.x - rectangleSize.x * 0.5f;
        float rectRightEdge = rectangleCenter.x + rectangleSize.x * 0.5f;
        float rectTopEdge = rectangleCenter.y + rectangleSize.y * 0.5f;
        float rectBottomEdge = rectangleCenter.y - rectangleSize.y * 0.5f;
        return point.x > rectLeftEdge && point.x < rectRightEdge && point.y > rectBottomEdge && point.y < rectTopEdge;
    }

    LineSegment Math::CalculateWorldLineSegment(const Matrix& worldTransform, const Vector2& start, const Vector2& end)
    {
        Matrix startTransform = worldTransform * Matrix::MakeTranslation(start);
        Matrix endTransform = worldTransform * Matrix::MakeTranslation(end);
        return LineSegment(startTransform.GetTranslation(), endTransform.GetTranslation());
    }

    float Math::DegreesToRadians(float aDegrees)
    {
        return aDegrees * 0.01745329251f;
    }

    float Math::RadiansToDegrees(float aRadians)
    {
        return aRadians * 57.2957795131f;
    }

    float Math::Clamp(float aValue, float aMin, float aMax)
    {
        if (aValue < aMin)
        {
            return aMin;
        }

        if (aValue > aMax)
        {
            return aMax;
        }

        return aValue;
    }

    Vector2 Math::Clamp(const Vector2& aValue, const Vector2& aMin, const Vector2& aMax)
    {
        if (aValue < aMin)
        {
            return aMin;
        }

        if (aValue > aMax)
        {
            return aMax;
        }

        return aValue;
    }

    int Math::RandomInt(int aMin, int aMax)
    {
        return s_Random.RandomInt(aMin, aMax);
    }

    float Math::RandomFloat()
    {
        return RandomFloat(0.0f, 1.0f);
    }

    float Math::RandomFloat(float aMin, float aMax)
    {
        return s_Random.RandomFloat(aMin, aMax);
    }

    double Math::RandomDouble()
    {
        return RandomDouble(0.0, 1.0);
    }

    double Math::RandomDouble(double aMin, double aMax)
    {
        return s_Random.RandomDouble(aMin, aMax);
    }

    bool Math::IsClose(float aValue, float aTarget, float aRange)
    {
        if (aValue > aTarget - aRange && aValue < aTarget + aRange)
        {
            return true;
        }

        return false;
    }
}
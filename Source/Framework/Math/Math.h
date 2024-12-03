#pragma once

#include "LineSegment.h"
#include "Matrix.h"
#include "Vector2.h"
#include "Random.h"


namespace GameDev2D
{
    struct Math
    {
        static const int BottomLeftCorner;
        static const int BottomRightCorner;
        static const int TopRightCorner;
        static const int TopLeftCorner;

        static void CalculateCorners(const Matrix& inTransform, const Vector2& inAnchor, const Vector2& inSize, Vector2 outCorners[4]);
        static Vector2 CalculateClosestPointOnLine(const LineSegment& lineSegment, const Vector2& point);
      
        static Vector2 RotatePoint(const Vector2& point, const Vector2& targetPoint, float targetRadians);

        static bool IsPointOnLineSegment(const LineSegment& lineSegment, const Vector2& point, float range = 0.1);
        static bool IsPointInsideCircle(const Vector2& circleCenter, float circleRadius, const Vector2& point);
        static bool IsPointInsideRectangle(const Vector2& rectangleCenter, const Vector2& rectangleSize, const Vector2& point); //Rectangle is not rotated

        static LineSegment CalculateWorldLineSegment(const Matrix& worldTransform, const Vector2& start, const Vector2& end);

        static float DegreesToRadians(float degrees);
        static float RadiansToDegrees(float radians);

        static float Clamp(float value, float min, float max);
        static Vector2 Clamp(const Vector2& value, const Vector2& min, const Vector2& max);

        static int RandomInt(int min, int max);   

        static float RandomFloat();                         //Returns a float value between 0.0 and 1.0
        static float RandomFloat(float min, float max);     //Returns a float value between a min value and a max value
    
        static double RandomDouble();                         //Returns a double value between 0.0 and 1.0
        static double RandomDouble(double min, double max);   //Returns a double value between a min value and a max value

        static bool IsClose(float value, float target, float range);

    private:
        static Random s_Random;
    };
}
#include "AABB.h"
#include <math.h>
#include <float.h>
#include "../../Source/Constants.h"

namespace GameDev2D
{
    AABB::AABB(const Vector2& lower, const Vector2& upper) :
        lower(lower),
        upper(upper)
    {}

    AABB::AABB() :
        lower(0.0f, 0.0f),
        upper(1.0f, 1.0f)
    {}

    Vector2 AABB::Center() const
    {
        return lower + GetHalfDimensions();
    }

    Vector2 AABB::GetDimensions() const
    {
        return upper - lower;
    }

    Vector2 AABB::GetHalfDimensions() const
    {
        return GetDimensions() * 0.5f;
    }

    bool AABB::DoesIntersectWithCircle(const LineSegment& circlePath, float circleRadius, Vector2& intersectionPoint, Vector2& collisionNormal)
    {
        // From: https://stackoverflow.com/questions/18704999/how-to-fix-circle-and-rectangle-overlap-in-collision-response/18790389#18790389
        // Converted to C++ and fixed a couple of bugs - Jimmy

        Vector2 start = circlePath.start;
        Vector2 end = circlePath.end;

        // T and B are flipped, this code assumes a top-left origin. - Jimmy
        float L = lower.x;
        float T = lower.y;
        float R = upper.x;
        float B = upper.y;

        // If the bounding box around the start and end points (+radius on all sides)
        //    does not intersect with the rectangle, definitely not an intersection.
        if ((fmaxf(start.x, end.x) + circleRadius < L) ||
            (fminf(start.x, end.x) - circleRadius > R) ||
            (fmaxf(start.y, end.y) + circleRadius < T) ||
            (fminf(start.y, end.y) - circleRadius > B))
        {
            return false;
        }

        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float invdx = (dx == 0.0f ? 0.0f : 1.0f / dx);
        float invdy = (dy == 0.0f ? 0.0f : 1.0f / dy);
        float cornerX = FLT_MAX;
        float cornerY = FLT_MAX;

        // Calculate intersection times with each side's plane.
        // Check each side's quadrant for single-side intersection.
        // Calculate Corner.

        /** Left Side **/
        // Does the circle go from the left side to the right side of the rectangle's left?
        if (start.x - circleRadius < L && end.x + circleRadius > L)
        {
            float ltime = ((L - circleRadius) - start.x) * invdx;
            if (ltime >= 0.0f && ltime <= 1.0f)
            {
                float ly = dy * ltime + start.y;
                // Does the collisions point lie on the left side?
                if (ly >= T && ly <= B)
                {
                    intersectionPoint.Set(dx * ltime + start.x, ly);
                    collisionNormal.Set(-1, 0);
                    return true;
                }
            }
            cornerX = L;
        }

        /** Right Side **/
        // Does the circle go from the right side to the left side of the rectangle's right?
        if (start.x + circleRadius > R && end.x - circleRadius < R)
        {
            float rtime = (start.x - (R + circleRadius)) * -invdx;
            if (rtime >= 0.0f && rtime <= 1.0f)
            {
                float ry = dy * rtime + start.y;
                // Does the collisions point lie on the right side?
                if (ry >= T && ry <= B)
                {
                    intersectionPoint.Set(dx * rtime + start.x, ry);
                    collisionNormal.Set(1, 0);
                    return true;
                }
            }

            if (cornerX == FLT_MAX)
            {
                cornerX = R;
            }
            else
            {
                // Bug Fix: - Jimmy
                // Pick the nearer of the Left or Right corner to the start.
                // This is a problem if the circle is larger than the aabb width.
                float distToRCorner = fabs(R - start.x);
                float distToLCorner = fabs(L - start.x);

                if (distToRCorner < distToLCorner)
                    cornerX = R;
                else
                    cornerX = L;
            }
        }

        /** Top Side **/
        // Does the circle go from the top side to the bottom side of the rectangle's top?
        if (start.y - circleRadius < T && end.y + circleRadius > T)
        {
            float ttime = ((T - circleRadius) - start.y) * invdy;
            if (ttime >= 0.0f && ttime <= 1.0f)
            {
                float tx = dx * ttime + start.x;
                // Does the collisions point lie on the top side?
                if (tx >= L && tx <= R)
                {
                    intersectionPoint.Set(tx, dy * ttime + start.y);
                    collisionNormal.Set(0, -1);
                    return true;
                }
            }
            cornerY = T;
        }

        /** Bottom Side **/
        // Does the circle go from the bottom side to the top side of the rectangle's bottom?
        if (start.y + circleRadius > B && end.y - circleRadius < B)
        {
            float btime = (start.y - (B + circleRadius)) * -invdy;
            if (btime >= 0.0f && btime <= 1.0f)
            {
                float bx = dx * btime + start.x;
                // Does the collisions point lie on the bottom side?
                if (bx >= L && bx <= R)
                {
                    intersectionPoint.Set(bx, dy * btime + start.y);
                    collisionNormal.Set(0, 1);
                    return true;
                }
            }

            if (cornerY == FLT_MAX)
            {
                cornerY = B;
            }
            else
            {
                // Bug Fix: - Jimmy
                // Pick the nearer of the Top or Bottom corner to the start.
                // This is a problem if the circle is larger than the aabb height.
                float distToBCorner = fabs(B - start.y);
                float distToTCorner = fabs(T - start.y);

                if (distToBCorner < distToTCorner)
                    cornerY = B;
                else
                    cornerY = T;
            }
        }

        // No intersection at all!
        if (cornerX == FLT_MAX && cornerY == FLT_MAX)
        {
            return false;
        }

        // Account for the times where we don't pass over a side but we do hit it's corner.
        if (cornerX != FLT_MAX && cornerY == FLT_MAX)
        {
            cornerY = (dy > 0.0f ? B : T);
        }
        if (cornerY != FLT_MAX && cornerX == FLT_MAX)
        {
            cornerX = (dx > 0.0f ? R : L);
        }

        /* Solve the triangle between the start, corner, and intersection point.
        *
        *           +-----------T-----------+
        *           |                       |
        *          L|                       |R
        *           |                       |
        *           C-----------B-----------+
        *          / \
        *         /   \r     _.-E
        *        /     \ _.-'
        *       /    _.-I
        *      / _.-'
        *     S-'
        *
        * S = start of circle's path
        * E = end of circle's path
        * LTRB = sides of the rectangle
        * I = {ix, iY} = point at which the circle intersects with the rectangle
        * C = corner of intersection (and collision point)
        * C=>I (r) = {nx, ny} = radius and intersection normal
        * S=>C = cornerdist
        * S=>I = intersectionDistance
        * S=>E = lineLength
        * <S = innerAngle
        * <I = angle1
        * <C = angle2
        */

        double inverseRadius = 1.0 / circleRadius;
        double lineLength = sqrt(dx * dx + dy * dy);
        double cornerdx = cornerX - start.x;
        double cornerdy = cornerY - start.y;
        double cornerDistance = sqrt(cornerdx * cornerdx + cornerdy * cornerdy);
        double innerAngle = acos((cornerdx * dx + cornerdy * dy) / (lineLength * cornerDistance));

        // If the circle is too close, no intersection.
        if (cornerDistance < circleRadius)
        {
            return false;
        }

        // If inner angle is zero, it's going to hit the corner straight on.
        if (innerAngle == 0.0f)
        {
            float time = (float)((cornerDistance - circleRadius) / lineLength);

            // If time is outside the boundaries, return null. This algorithm can
            // return a negative time which indicates a previous intersection, and
            // can also return a time > 1.0f which can predict a corner intersection.
            if (time > 1.0f || time < 0.0f)
            {
                return false;
            }

            float ix = time * dx + start.x;
            float iy = time * dy + start.y;
            float nx = (float)(cornerdx / cornerDistance);
            float ny = (float)(cornerdy / cornerDistance);

            intersectionPoint.Set(ix, iy);
            collisionNormal.Set(nx, ny);
            return true;
        }

        double innerAngleSin = sin(innerAngle);
        double angle1Sin = innerAngleSin * cornerDistance * inverseRadius;

        // The angle is too large, there cannot be an intersection.
        if (abs(angle1Sin) > 1.0f)
        {
            return false;
        }

        double angle1 = M_PI - asin(angle1Sin);
        double angle2 = M_PI - innerAngle - angle1;
        double intersectionDistance = circleRadius * sin(angle2) / innerAngleSin;

        // Solve for time.
        float time = (float)(intersectionDistance / lineLength);

        // If time is outside the boundaries, return null. This algorithm can
        // return a negative time which indicates a previous intersection, and
        // can also return a time > 1.0f which can predict a corner intersection.
        if (time > 1.0f || time < 0.0f)
        {
            return false;
        }

        // Solve the intersection and normal.
        float ix = time * dx + start.x;
        float iy = time * dy + start.y;
        float nx = (float)((ix - cornerX) * inverseRadius);
        float ny = (float)((iy - cornerY) * inverseRadius);

        intersectionPoint.Set(ix, iy);
        collisionNormal.Set(nx, ny);
        return true;
    }

    bool AABB::Intersects(const AABB& other) const
    {
        if (upper.x < other.lower.x)
            return false;

        if (upper.y < other.lower.y)
            return false;

        if (lower.x > other.upper.x)
            return false;

        if (lower.y > other.upper.y)
            return false;

        return true;
    }

    bool AABB::Contains(const AABB& other) const
    {
        if (other.lower.x >= lower.x &&
            other.upper.x <= upper.x &&
            other.lower.y >= lower.y &&
            other.upper.y <= upper.y)
            return true;

        return false;
    }

    bool AABB::ResolveCollision(AABB& other)
    {
        if (!Intersects(other))
            return false;

        float overlapX = std::fminf(upper.x - other.lower.x, other.upper.x - lower.x);
        float overlapY = std::fminf(upper.y - other.lower.y, other.upper.y - lower.y);

        if (overlapX < overlapY) {
            if (lower.x < other.lower.x) {
                lower.x -= overlapX / 2;
                upper.x -= overlapX / 2;
                other.lower.x += overlapX / 2;
                other.upper.x += overlapX / 2;
            }
            else {
                lower.x += overlapX / 2;
                upper.x += overlapX / 2;
                other.lower.x -= overlapX / 2;
                other.upper.x -= overlapX / 2;
            }
        }
        else {
            if (lower.y < other.lower.y) {
                lower.y -= overlapY / 2;
                upper.y -= overlapY / 2;
                other.lower.y += overlapY / 2;
                other.upper.y += overlapY / 2;
            }
            else {
                lower.y += overlapY / 2;
                upper.y += overlapY / 2;
                other.lower.y -= overlapY / 2;
                other.upper.y -= overlapY / 2;
            }
        }
        lower = { RoundM(lower.x, c_cellSize), RoundM(lower.y, c_cellSize)};
        upper = { RoundM(upper.x, c_cellSize), RoundM(upper.y, c_cellSize)};
        return true;
    }

    float AABB::RoundM(float n, float m)
    {
        return floorf(((n + m - 1) / m)) * m;
    }
}
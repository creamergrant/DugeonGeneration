//
//  Color.cpp
//  GameDev2D
//
//  Created by Bradley Flood on 2013-07-16.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//


#include "Color.h"
#include "../Debug/Log.h"
#include "../Math/Math.h"
#include <math.h>


namespace GameDev2D
{
    Color::Color() :
        r(1.0f),
        g(1.0f),
        b(1.0f),
        a(1.0f)
    {

    }
    
    Color::Color(const Color& aColor) :
        r(aColor.r),
        g(aColor.g),
        b(aColor.b),
        a(aColor.a)
    {

    }

    Color::Color(float aRed, float aGreen, float aBlue, float aAlpha) :
        r(1.0f),
        g(1.0f),
        b(1.0f),
        a(1.0f)
    {
        Set(aRed, aGreen, aBlue, aAlpha);
    }
    
    Color::Color(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha) :
        r(1.0f),
        g(1.0f),
        b(1.0f),
        a(1.0f)
    {
        Set(aRed, aGreen, aBlue, aAlpha);
    }
    
    Color::Color(unsigned int aColor) :
        r(1.0f),
        g(1.0f),
        b(1.0f),
        a(1.0f)
    {
        Set(aColor);
    }
    
    void Color::Set(float aRed, float aGreen, float aBlue, float aAlpha)
    {
        r = aRed;
        g = aGreen;
        b = aBlue;
        a = aAlpha;
    }
    
    void Color::Set(unsigned char aRed, unsigned char aGreen, unsigned char aBlue, unsigned char aAlpha)
    {
        Set((float)aRed / 255.0f, (float)aGreen / 255.0f, (float)aBlue / 255.0f, (float)aAlpha / 255.0f);
    }
    
    void Color::Set(unsigned int aColor)
    {
        r = (float)((aColor >> 24) & 0xff) / (float)(0xff);
        g = (float)((aColor >> 16) & 0xff) / (float)(0xff);
        b = (float)((aColor >> 8) & 0xff) / (float)(0xff);
        a = (float)((aColor)& 0xff) / (float)(0xff);
    }
    
    bool Color::operator==(const Color& aColor) const
    {
        if (r == aColor.r && g == aColor.g && b == aColor.b && a == aColor.a)
        {
            return true;
        }

        return false;
    }
    
    bool Color::operator!=(const Color& aColor) const
    {
        return !(*this == aColor);
    }
    
    void Color::InvertColor()
    {
        r = (1.0f - r);
        g = (1.0f - g);
        b = (1.0f - b);
    }

    bool Color::IsValid(Color aColor)
    {
        if (aColor.r > 1.0f || aColor.r < 0.0f) return false;
        if (aColor.g > 1.0f || aColor.g < 0.0f) return false;
        if (aColor.b > 1.0f || aColor.b < 0.0f) return false;
        if (aColor.a > 1.0f || aColor.a < 0.0f) return false;

        return true;
    }

    Color Color::Random(float aAlpha)
    {
        return Color(Math::RandomFloat(), Math::RandomFloat(), Math::RandomFloat(), aAlpha);
    }
}

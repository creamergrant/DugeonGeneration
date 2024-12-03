#ifndef __GameDev2D__Color__
#define __GameDev2D__Color__

namespace GameDev2D
{
    //Basic color class to manage RGBA color data. Color components are stored as 
    //float variables and have a range of 0.0f - 1.0f there is nothing to stop 
    //you from breaking these ranges, so be aware of that
    struct Color
    {
        //Create the color white
        Color();

        //Copies another Color in the newly created Color
        Color(const Color& color);
        
        //float range: 0.0f - 1.0f
        Color(float red, float green, float blue, float alpha = 1.0f);
        
        //unsigned char range: 0 - 255
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);
        
        //Hexadecimal RGBA
        Color(unsigned int color);

        //float range: 0.0f - 1.0f
        void Set(float red, float green, float blue, float alpha);
        
        //unsigned char range: 0 - 255
        void Set(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);
        
        //hexadecimal RGBA
        void Set(unsigned int color);

        //Overloaded operators
        bool operator==(const Color& color) const;
        bool operator!=(const Color& color) const;
        
        //Inverts the color
        void InvertColor();

        //Returns wether a color has valid color values
        static bool IsValid(Color color);
        
        //Returns a Random color
        static Color Random(float alpha = 1.0f);

        //Member variables
        float r;
        float g;
        float b;
        float a;
    };
}

#endif

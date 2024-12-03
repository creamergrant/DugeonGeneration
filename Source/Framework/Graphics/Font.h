#pragma once

#include "GraphicTypes.h"
#include <unordered_map>
#include <string>


namespace GameDev2D
{
    class Texture;
    struct FontData;
    struct GlyphData;

    class Font
    {
    public:
        Font(ImageData& imageData, FontData& fontData);
        ~Font();

        Texture* GetTexture();
        
        GlyphData GetGlyph(char character);
        std::string GetName();
        std::string GetCharacterSet();
        unsigned int GetLineHeight();
        unsigned int GetBaseline();
        unsigned int GetSize();

    private:
        Texture* m_Texture;
        std::unordered_map<char, GlyphData> m_GlyphData;
        std::string m_Name;
        std::string m_CharacterSet;
        unsigned int m_LineHeight;
        unsigned int m_Baseline;
        unsigned int m_Size;
    };

    struct GlyphData
    {
        GlyphData();

        unsigned char width;      // width of the glyph in pixels
        unsigned char height;     // height of the glyph in pixels
        unsigned char advanceX;   // number of pixels to advance on x axis
        char bearingX;            // x offset of top-left corner from x axis
        char bearingY;            // y offset of top-left corner from y axis
        Rect frame;
    };

    struct FontData
    {
        FontData();

        std::unordered_map<char, GlyphData> glyphData;
        std::string name;
        std::string characterSet;
        unsigned int lineHeight;
        unsigned int baseline;
        unsigned int size;
    };
}
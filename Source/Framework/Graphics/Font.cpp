#include "Font.h"
#include "Texture.h"


namespace GameDev2D
{
    Font::Font(ImageData& imageData, FontData& fontData) :
        m_Texture(nullptr),
        m_GlyphData(fontData.glyphData),
        m_Name(fontData.name),
        m_CharacterSet(fontData.characterSet),
        m_LineHeight(fontData.lineHeight),
        m_Baseline(fontData.baseline),
        m_Size(fontData.size)
    {
        m_Texture = new Texture(imageData);
    }

    Font::~Font()
    {
        if (m_Texture != nullptr)
        {
            delete m_Texture;
            m_Texture = nullptr;
        }
    }

    Texture* Font::GetTexture()
    {
        return m_Texture;
    }

    GlyphData Font::GetGlyph(char character)
    {
        return m_GlyphData[character];
    }

    std::string Font::GetName()
    {
        return m_Name;
    }

    std::string Font::GetCharacterSet()
    {
        return m_CharacterSet;
    }

    unsigned int Font::GetLineHeight()
    {
        return m_LineHeight;
    }

    unsigned int Font::GetBaseline()
    {
        return m_Baseline;
    }

    unsigned int Font::GetSize()
    {
        return m_Size;
    }

    GlyphData::GlyphData() :
        width(0),
        height(0),
        advanceX(0),
        bearingX(0),
        bearingY(0),
        frame()
    {
    }

    FontData::FontData() :
        lineHeight(0),
        baseline(0),
        size(0)
    {
    }
}
#include "SpriteFont.h"
#include "GraphicTypes.h"
#include "../Application/Application.h"
#include "../Utils/Text/Text.h"
#include <fstream>


namespace GameDev2D
{
	SpriteFont::SpriteFont(const std::string& aFilename) : Sprite(aFilename),
		m_Font(Application::Get().GetResourceManager().GetFontCache().Get(aFilename)),
		m_Text(),
		m_Size(Vector2(0.0f, 0.0f)),
		m_Justification(Justification::Left),
		m_CharacterSpacing(0.0f),
		m_LineCount(0)
	{
		SetTexture(m_Font->GetTexture());
	}

	SpriteFont::SpriteFont() : Sprite(),
		m_Font(Application::Get().GetResourceManager().GetFontCache().GetDefaultResource()),
		m_Text(),
		m_Size(Vector2(0.0f, 0.0f)),
		m_Justification(Justification::Left),
		m_CharacterSpacing(0.0f),
		m_LineCount(0)
	{
		SetTexture(m_Font->GetTexture());
	}

	SpriteFont::~SpriteFont()
	{

	}

	float SpriteFont::GetWidth()
	{
		return m_Size.x;
	}

	float SpriteFont::GetHeight()
	{
		return m_Size.y;
	}

	void SpriteFont::SetColor(Color aColor)
	{
		Sprite::SetColor(aColor);

		for (unsigned int i = 0; i < m_CharacterData.size(); i++)
		{
			m_CharacterData.at(i).color = aColor;
		}
	}

	void SpriteFont::SetFont(const std::string& fontFile)
	{
		m_Font = Application::Get().GetResourceManager().GetFontCache().Get(fontFile);
		SetTexture(m_Font->GetTexture());
	}

	Font* SpriteFont::GetFont()
	{
		return m_Font;
	}

	void SpriteFont::SetText(const std::string& aText)
	{
		//Is the text the exact same, if so return
		if (aText != m_Text)
		{
			//Set the text
			m_Text = std::string(aText);

			//Clear the character data
			m_CharacterData.clear();

			//Add a CharacterData object for each letter
			for (unsigned int i = 0; i < m_Text.length(); i++)
			{
				m_CharacterData.push_back(CharacterData(GetColor()));
			}

			//Re-calculate the size
			CalculateSize();
		}
	}

	std::string SpriteFont::GetText()
	{
		return m_Text;
	}

	void SpriteFont::SetCharacterSpacing(float aCharacterSpacing)
	{
		if (m_CharacterSpacing != aCharacterSpacing)
		{
			//Set the character spacing
			m_CharacterSpacing = aCharacterSpacing;

			//Re-calculate the size
			CalculateSize();
		}
	}

	float SpriteFont::GetCharacterSpacing()
	{
		return m_CharacterSpacing;
	}

	unsigned int SpriteFont::GetNumberOfLines()
	{
		return m_LineCount;
	}

	float SpriteFont::GetLineWidth(unsigned int lineIndex)
	{
		return m_LineWidth.at(lineIndex);
	}

	void SpriteFont::SetJustification(SpriteFont::Justification aJustification)
	{
		//Safety check that the justification isn't already set
		if (aJustification != m_Justification)
		{
			//Set the justification
			m_Justification = aJustification;
		}
	}

	SpriteFont::Justification SpriteFont::GetJustification()
	{
		return m_Justification;
	}

	void SpriteFont::CalculateSize()
	{
		//Safety check that the font pointer is null
		if (m_Font == nullptr)
		{
			return;
		}

		//Used to track the max width and height
		float x = 0.0f;
		Vector2 maxSize(0.0f, static_cast<float>(m_Font->GetLineHeight()));

		//Set the line count
		m_LineCount = Text::NumberOfLines(m_Text);

		//Clear the line width's vector
		m_LineWidth.clear();

		//Cycle through all the characters in the text string
		for (unsigned int i = 0; i < m_Text.length(); i++)
		{
			//Did we reach a new line?
			if (m_Text.at(i) == '\n')
			{
				//Add the line width to the line widths vector
				m_LineWidth.push_back(x);

				//Calculate the max width and increment the max height
				maxSize.x = fmaxf(x, maxSize.x);
				maxSize.y += m_Font->GetLineHeight();

				//Set x back to zero
				x = 0.0f;
				continue;
			}

			//Advance the x, by the x-advance of the character
			x += m_Font->GetGlyph(m_Text.at(i)).advanceX + GetCharacterSpacing();
		}

		//We are done, add the line width to the line widths vector
		m_LineWidth.push_back(x);

		//Calculate the max width
		maxSize.x = fmaxf(x, maxSize.x);

		//Set the size
		m_Size = maxSize;
	}

	std::vector<SpriteFont::CharacterData>& SpriteFont::GetCharacterData()
	{
		return m_CharacterData;
	}
}
#pragma once

#include "Sprite.h"
#include "Font.h"


namespace GameDev2D
{
	class SpriteFont : public Sprite
	{
	public:
		//A font should only be generated through the ResourceManager
		SpriteFont(const std::string& filename);
		SpriteFont();
		~SpriteFont();

		//Returns the size of the label
		float GetWidth() override;
		float GetHeight() override;

		//Sets the Color of all the characters
		void SetColor(Color color);

		//Changes the internal font that's being used
		void SetFont(const std::string& fontFile);
		Font* GetFont();

		//Sets the text that the Label will display
		void SetText(const std::string& text);

		//Returns the text the Label is displaying
		std::string GetText();

		//Set the spacing between each character
		void SetCharacterSpacing(float characterSpacing);

		//Returns the spacing between each character
		float GetCharacterSpacing();

		//
		unsigned int GetNumberOfLines();
		
		//
		float GetLineWidth(unsigned int lineIndex);

		//Label Justification text can be left, center and right justified
		enum class Justification
		{
			Left = 0,
			Center,
			Right
		};

		//Sets the justification of the Label
		void SetJustification(Justification justification);

		//Returns the justification of the Label
		Justification GetJustification();

		//
		struct CharacterData
		{
			CharacterData(Color color) :
				color(color),
				scale(1.0f, 1.0f),
				anchor(0.0f, 0.0f),
				angle(0.0f)
			{}

			CharacterData() :
				color(ColorList::White),
				scale(1.0f, 1.0f),
				anchor(0.0f, 0.0f),
				angle(0.0f)
			{}

			CharacterData(const CharacterData& characterData) :
				color(characterData.color),
				scale(characterData.scale),
				anchor(characterData.anchor),
				angle(characterData.angle)
			{}

			Color color;
			Vector2 scale;
			Vector2 anchor;
			float angle;
		};

		//Returns a reference to the CharacterData so that you can modify each character's data individually
		std::vector<CharacterData>& GetCharacterData();

	private:
		//Conveniance method to calculate the size of the SpriteFont, based on the text
		void CalculateSize();

		//Member variables
		Font* m_Font;
		std::string m_Text;
		std::vector<float> m_LineWidth;
		std::vector<CharacterData> m_CharacterData;
		Vector2 m_Size;
		Justification m_Justification;
		float m_CharacterSpacing;
		unsigned int m_LineCount;
	};
}
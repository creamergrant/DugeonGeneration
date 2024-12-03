#include "SpriteRenderer.h"
#include "Camera.h"
#include "Sprite.h"
#include "SpriteFont.h"
#include "Texture.h"
#include "../Application/Application.h"
#include "../Resources/ResourceManager.h"
#include "../Utils/Text/Text.h"


namespace GameDev2D
{
	SpriteRenderer::SpriteRenderer() :
		m_TextureSlotIndex(1), // 0 = white texture
		m_QuadIndexCount(0),
		m_QuadVertexBufferBase(nullptr),
		m_QuadVertexBufferPtr(nullptr),
		m_QuadVertexPositions{},
		m_RenderSpriteOutline(false)
	{
		m_QuadVertexArray = std::make_shared<VertexArray>();

		m_QuadVertexBuffer = std::make_shared<VertexBuffer>(kMaxVertices * sizeof(QuadVertex));
		m_QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Vertices" },
			{ ShaderDataType::Float2, "a_TextureUV" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float, "a_TexureIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
			});

		m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);

		m_QuadVertexBufferBase = new QuadVertex[kMaxVertices];
		unsigned int* quadIndices = new unsigned int[kMaxIndices];

		unsigned int offset = 0;
		for (unsigned int i = 0; i < kMaxIndices; i += kQuadIndexCount)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += kQuadVertexCount;
		}

		std::shared_ptr<IndexBuffer> quadIB = std::make_shared<IndexBuffer>(quadIndices, kMaxIndices);
		m_QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		ImageData imageData(PixelFormat(PixelLayout::RGBA, PixelSize::UnsignedByte), 1, 1);
		imageData.data.push_back(0xff);
		imageData.data.push_back(0xff);
		imageData.data.push_back(0xff);
		imageData.data.push_back(0xff);
		m_WhiteTexture = std::make_shared<Texture>(imageData);

		int samplers[MAX_TEXTURE_SLOTS];
		for (unsigned int i = 0; i < MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		m_TextureShader = Application::Get().GetResourceManager().GetShaderCache().GetShared(SPRITE_RENDERER_SHADER_KEY);
		m_TextureShader->Bind();
		m_TextureShader->SetIntArray("u_Textures", samplers, MAX_TEXTURE_SLOTS);

		//Set first texture slot to 0
		m_TextureSlots[0] = m_WhiteTexture.get();

		m_QuadVertexPositions[0] = { 0.0f, 0.0f };
		m_QuadVertexPositions[1] = { 1.0f, 0.0f };
		m_QuadVertexPositions[2] = { 1.0f, 1.0f };
		m_QuadVertexPositions[3] = { 0.0f, 1.0f };
	}

	SpriteRenderer::~SpriteRenderer()
	{
		if (m_QuadVertexBufferBase != nullptr)
		{
			delete[] m_QuadVertexBufferBase;
			m_QuadVertexBufferBase = nullptr;
		}
	}

	RendererType SpriteRenderer::GetType()
	{
		return RendererType::Sprite;
	}

	void SpriteRenderer::BeginScene(Camera* camera)
	{
		m_TextureShader->Bind();
		m_TextureShader->SetMat4("u_ViewProjection", camera->GetViewProjectionMatrix());

		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

		m_TextureSlotIndex = 1;
	}

	void SpriteRenderer::EndScene()
	{
		unsigned int dataSize = (unsigned int)((unsigned char*)m_QuadVertexBufferPtr - (unsigned char*)m_QuadVertexBufferBase);
		m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, dataSize);

		Flush();
	}

	void SpriteRenderer::Flush()
	{
		if (m_QuadIndexCount == 0)
			return;

		for (unsigned int i = 0; i < m_TextureSlotIndex; i++)
		{
			m_TextureSlots[i]->Bind(i);
		}

		m_QuadVertexArray->Bind();
		Application::Get().GetGraphics().DrawElements(RenderMode::Triangles, m_QuadVertexArray, m_QuadIndexCount);
		m_TotalDrawCalls++;
		m_FrameDrawCalls++;
	}

	void SpriteRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor)
	{
		Matrix transform = Matrix::MakeTranslation(position);
		RenderQuad(transform, size, anchor, fillColor);
	}

	void SpriteRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		Matrix transform = Matrix::MakeTranslation(position);
		RenderQuad(transform, size, anchor, fillColor);
		RenderOutline(transform, size, anchor, outlineColor, outlineSize);
	}

	void SpriteRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor)
	{
		Matrix translationMatrix = Matrix::MakeTranslation(position);
		Matrix rotationMatrix = Matrix::MakeRotation(radians);
		Matrix transform = translationMatrix * rotationMatrix;
		RenderQuad(transform, size, anchor, fillColor);
	}

	void SpriteRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		Matrix translationMatrix = Matrix::MakeTranslation(position);
		Matrix rotationMatrix = Matrix::MakeRotation(radians);
		Matrix transform = translationMatrix * rotationMatrix;
		RenderQuad(transform, size, anchor, fillColor);
		RenderOutline(transform, size, anchor, outlineColor, outlineSize);
	}

	void SpriteRenderer::RenderSprite(Sprite* sprite)
	{
		RenderSprite(*sprite);
	}

	void SpriteRenderer::RenderSprite(Sprite& sprite)
	{
		Vector2 uvs[4];
		sprite.GetUVs(uvs);
		RenderTexture(sprite.GetTexture(), sprite.GetWorldTransform(), uvs, sprite.GetSize(), 1.0f, sprite.GetAnchor(), sprite.GetColor());
	
#if DEBUG
		if (m_RenderSpriteOutline)
		{
			RenderOutline(sprite.GetWorldTransform(), sprite.GetSize() * sprite.GetWorldScale(), sprite.GetAnchor(), kSpriteOutlineColor, kSpriteOutlineSize);
			RenderAnchor(sprite.GetWorldTransform());
		}
#endif
	}

	void SpriteRenderer::RenderSpriteFont(SpriteFont* spriteFont)
	{
		RenderSpriteFont(*spriteFont);
	}

	void SpriteRenderer::RenderSpriteFont(SpriteFont& spriteFont)
	{
		//Is there any text to render?
		if (spriteFont.GetText().length() == 0)
		{
			return;
		}

		//calculate the baseline and origin for the label
		unsigned int baseline = spriteFont.GetFont()->GetLineHeight() - spriteFont.GetFont()->GetBaseline();
		unsigned int numberOfLines = Text::NumberOfLines(spriteFont.GetText());
		Vector2 origin = Vector2(0.0f, 0.0f);
		Vector2 position = Vector2(0.0f, 0.0f);
		position.y += baseline + (spriteFont.GetFont()->GetLineHeight() * (numberOfLines - 1));
		int lineIndex = 0;

		//What justification are we dealing with
		if (spriteFont.GetJustification() == SpriteFont::Justification::Left)
		{
			position.x = 0.0f;
		}
		else if (spriteFont.GetJustification() == SpriteFont::Justification::Center)
		{
			position.x = (spriteFont.GetWidth() - spriteFont.GetLineWidth(lineIndex)) * 0.5f;
		}
		else if (spriteFont.GetJustification() == SpriteFont::Justification::Right)
		{
			position.x = spriteFont.GetWidth() - spriteFont.GetLineWidth(lineIndex);
		}

		//Cycle through the characters in the text label
		for (unsigned int i = 0; i < spriteFont.GetText().length(); i++)
		{
			//Did we reach a new line?
			if (spriteFont.GetText().at(i) == '\n')
			{
				//Increment the line index
				lineIndex++;

				//Calculate the line's origin based on the justification
				if (spriteFont.GetJustification() == SpriteFont::Justification::Left)
				{
					position.x = 0.0f;
				}
				else if (spriteFont.GetJustification() == SpriteFont::Justification::Center)
				{
					position.x = (spriteFont.GetWidth() - spriteFont.GetLineWidth(lineIndex)) * 0.5f;
				}
				else if (spriteFont.GetJustification() == SpriteFont::Justification::Right)
				{
					position.x = spriteFont.GetWidth() - spriteFont.GetLineWidth(lineIndex);
				}

				//Set the y line origin based on the line height of the font
				position.y -= spriteFont.GetFont()->GetLineHeight();
				continue;
			}

			//Set the frame
			GlyphData glyph = spriteFont.GetFont()->GetGlyph(spriteFont.GetText().at(i));
			Rect characterRect = glyph.frame;
			spriteFont.SetFrame(characterRect);
			Vector2 uvs[4];
			spriteFont.GetUVs(uvs);

			//Rect characterRect = m_Font->GetTextureRectForCharacter();
			Vector2 anchorOffset = Vector2(-spriteFont.GetWidth() * spriteFont.GetAnchor().x, -spriteFont.GetHeight() * spriteFont.GetAnchor().y);
			Vector2 charPosition = position + anchorOffset;
			charPosition.x += glyph.bearingX;
			charPosition.y += glyph.bearingY - characterRect.size.y;

			//Calculate the Matrix
			Matrix parent = spriteFont.GetWorldTransform();
			Matrix local = Matrix::Make(charPosition, spriteFont.GetCharacterData().at(i).angle, spriteFont.GetCharacterData().at(i).scale);

			RenderTexture(spriteFont.GetTexture(), parent * local, uvs, characterRect.size, 1.0f, spriteFont.GetCharacterData().at(i).anchor, spriteFont.GetCharacterData().at(i).color);

			//Increment the position
			position.x += glyph.advanceX + spriteFont.GetCharacterSpacing();
		}
	}

	void SpriteRenderer::EnableDebugSpriteOutline(bool enable)
	{
		m_RenderSpriteOutline = enable;
	}

	bool SpriteRenderer::IsDebugSpriteOutlineEnabled()
	{
		return m_RenderSpriteOutline;
	}

	void SpriteRenderer::RenderQuad(const Matrix& transform, const Vector2& size, const Vector2& anchor, const Color& fillColor)
	{
		if (m_QuadIndexCount + kQuadIndexCount >= kMaxIndices)
		{
			FlushAndReset();
		}

		const Vector2 uvs[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		const float textureIndex = 0.0f; // White Texture
		const float tilingFactor = 1.0f;

		Vector2 offset;
		for (unsigned int i = 0; i < kQuadVertexCount; i++)
		{
			offset = (m_QuadVertexPositions[i] * size) - Vector2(size.x * anchor.x, size.y * anchor.y);
			m_QuadVertexBufferPtr->vertices = transform * offset;
			m_QuadVertexBufferPtr->uv = uvs[i];
			m_QuadVertexBufferPtr->color = fillColor;
			m_QuadVertexBufferPtr->texureIndex = textureIndex;
			m_QuadVertexBufferPtr->tilingFactor = tilingFactor;
			m_QuadVertexBufferPtr++;
		}

		m_QuadIndexCount += kQuadIndexCount;
	}

	void SpriteRenderer::RenderTexture(Texture* texture, const Matrix& transform, Vector2 uvs[4], const Vector2& size, float tilingFactor, const Vector2 anchor, const Color& tintColor)
	{
		if (m_QuadIndexCount + kQuadIndexCount >= kMaxIndices)
		{
			FlushAndReset();
		}

		float textureIndex = 0.0f;
		for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
		{
			if (*m_TextureSlots[i] == *texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (m_TextureSlotIndex >= MAX_TEXTURE_SLOTS)
			{
				FlushAndReset();
			}

			textureIndex = (float)m_TextureSlotIndex;
			m_TextureSlots[m_TextureSlotIndex] = texture;
			m_TextureSlotIndex++;
		}

		Vector2 offset;
		for (unsigned int i = 0; i < kQuadVertexCount; i++)
		{
			offset = (m_QuadVertexPositions[i] * size) - Vector2(size.x * anchor.x, size.y * anchor.y);
			m_QuadVertexBufferPtr->vertices = transform * offset;
			m_QuadVertexBufferPtr->uv = uvs[i];
			m_QuadVertexBufferPtr->color = tintColor;
			m_QuadVertexBufferPtr->texureIndex = textureIndex;
			m_QuadVertexBufferPtr->tilingFactor = tilingFactor;
			m_QuadVertexBufferPtr++;
		}

		m_QuadIndexCount += kQuadIndexCount;
	}

	void SpriteRenderer::RenderOutline(const Matrix& transform, const Vector2& size, const Vector2& anchor, const Color& outlineColor, float outlineSize)
	{
		Vector2 quadCorners[4];
		Math::CalculateCorners(transform, anchor, size, quadCorners);

		float quadRadians(transform.GetRadians());
		Vector2 quadMidpoint = LineSegment(quadCorners[Math::BottomLeftCorner], quadCorners[Math::TopRightCorner]).Midpoint();
		Vector2 horizontalSize(size.x + outlineSize * 2.0f, outlineSize);
		Vector2 verticalSize(outlineSize, size.y);
		Vector2 outlineAnchor(0.5f, 0.5f);

		//Left
		{
			Vector2 leftEdgeMidpoint = LineSegment(quadCorners[Math::BottomLeftCorner], quadCorners[Math::TopLeftCorner]).Midpoint();
			Vector2 direction = Vector2(quadMidpoint - leftEdgeMidpoint).Normalized();
			Vector2 leftOutlinePosition = leftEdgeMidpoint - direction * (outlineSize * 0.5f);
			Matrix translationMatrix = Matrix::MakeTranslation(leftOutlinePosition);
			Matrix rotationMatrix = Matrix::MakeRotation(quadRadians);
			Matrix transform = translationMatrix * rotationMatrix;
			RenderQuad(transform, verticalSize, outlineAnchor, outlineColor);
		}

		//Right
		{
			Vector2 rightEdgeMidpoint = LineSegment(quadCorners[Math::BottomRightCorner], quadCorners[Math::TopRightCorner]).Midpoint();
			Vector2 direction = Vector2(quadMidpoint - rightEdgeMidpoint).Normalized();
			Vector2 rightOutlinePosition = rightEdgeMidpoint - direction * (outlineSize * 0.5f);
			Matrix translationMatrix = Matrix::MakeTranslation(rightOutlinePosition);
			Matrix rotationMatrix = Matrix::MakeRotation(quadRadians);
			Matrix transform = translationMatrix * rotationMatrix;
			RenderQuad(transform, verticalSize, outlineAnchor, outlineColor);
		}

		//Top
		{
			Vector2 topEdgeMidpoint = LineSegment(quadCorners[Math::TopLeftCorner], quadCorners[Math::TopRightCorner]).Midpoint();
			Vector2 direction = Vector2(quadMidpoint - topEdgeMidpoint).Normalized();
			Vector2 topOutlinePosition = topEdgeMidpoint - direction * (outlineSize * 0.5f);
			Matrix translationMatrix = Matrix::MakeTranslation(topOutlinePosition);
			Matrix rotationMatrix = Matrix::MakeRotation(quadRadians);
			Matrix transform = translationMatrix * rotationMatrix;
			RenderQuad(transform, horizontalSize, outlineAnchor, outlineColor);
		}

		//Bottom
		{
			Vector2 bottomEdgeMidpoint = LineSegment(quadCorners[Math::BottomLeftCorner], quadCorners[Math::BottomRightCorner]).Midpoint();
			Vector2 direction = Vector2(quadMidpoint - bottomEdgeMidpoint).Normalized();
			Vector2 bottomOutlinePosition = bottomEdgeMidpoint - direction * (outlineSize * 0.5f);
			Matrix translationMatrix = Matrix::MakeTranslation(bottomOutlinePosition);
			Matrix rotationMatrix = Matrix::MakeRotation(quadRadians);
			Matrix transform = translationMatrix * rotationMatrix;
			RenderQuad(transform, horizontalSize, outlineAnchor, outlineColor);
		}

	}

	void SpriteRenderer::RenderAnchor(const Matrix& transform)
	{
		RenderQuad(transform, Vector2(kAnchorSize.x, kAnchorSize.y), Vector2(0.0f, 0.0f), kAnchorColor);
		RenderQuad(transform, Vector2(kAnchorSize.y, kAnchorSize.x), Vector2(0.0f, 0.0f), kAnchorColor);
	}

	void SpriteRenderer::FlushAndReset()
	{
		EndScene();

		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
		m_TextureSlotIndex = 1;
	}
}
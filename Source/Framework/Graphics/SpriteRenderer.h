#pragma once

#include "IRenderer.h"
#include "Buffer.h"
#include "Color.h"
#include "ColorList.h"
#include "Shader.h"
#include "VertexArray.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include <array>


namespace GameDev2D
{
	class Camera;
	class Sprite;
	class SpriteFont;
	class Texture;

	const unsigned int MAX_TEXTURE_SLOTS = 32;

	class SpriteRenderer : public IRenderer
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		RendererType GetType() override;

		void BeginScene(Camera* camera) override;
		void EndScene() override;

		void RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor);
		void RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize);
		
		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor);
		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize);

		void RenderSprite(Sprite* sprite);
		void RenderSprite(Sprite& sprite);

		void RenderSpriteFont(SpriteFont* spriteFont);
		void RenderSpriteFont(SpriteFont& spriteFont);

		void EnableDebugSpriteOutline(bool enable);
		bool IsDebugSpriteOutlineEnabled();

	protected:
		friend class BatchRenderer;
		void Flush() override;
		void FlushAndReset() override;

	private:
		void RenderQuad(const Matrix& transform, const Vector2& size, const Vector2& anchor, const Color& fillColor);
		void RenderTexture(Texture* texture, const Matrix& transform, Vector2 uvs[4], const Vector2& size, float tilingFactor, const Vector2 anchor, const Color& tintColor);
		void RenderOutline(const Matrix& transform, const Vector2& size, const Vector2& anchor, const Color& outlineColor, float outlineSize);
		void RenderAnchor(const Matrix& transform);

		struct QuadVertex
		{
			Vector2 vertices;
			Vector2 uv;
			Color color;
			float texureIndex;
			float tilingFactor;
		};

		std::shared_ptr<VertexArray> m_QuadVertexArray;
		std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;
		std::shared_ptr<Shader> m_TextureShader;
		std::shared_ptr<Texture> m_WhiteTexture;
		std::array<Texture*, MAX_TEXTURE_SLOTS> m_TextureSlots;
		unsigned int m_TextureSlotIndex;
		unsigned int m_QuadIndexCount;
		QuadVertex* m_QuadVertexBufferBase;
		QuadVertex* m_QuadVertexBufferPtr;
		Vector2 m_QuadVertexPositions[4];
		bool m_RenderSpriteOutline;

		const unsigned int kMaxQuads = 20000;
		const unsigned int kQuadVertexCount = 4;
		const unsigned int kMaxVertices = kMaxQuads * kQuadVertexCount;
		const unsigned int kQuadIndexCount = 6;
		const unsigned int kMaxIndices = kMaxQuads * kQuadIndexCount;
		const Color kSpriteOutlineColor = ColorList::Blue;
		const float kSpriteOutlineSize = 2.0f;
		const Color kAnchorColor = ColorList::Green;
		const Vector2 kAnchorSize = Vector2(12.0f, 2.0f);
	};
}
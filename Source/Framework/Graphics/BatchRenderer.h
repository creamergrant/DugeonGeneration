#pragma once

#include "SpriteRenderer.h"
#include "LineRenderer.h"
#include "PointRenderer.h"
#include "CircleRenderer.h"
#include <array>


namespace GameDev2D
{
	class Camera;
	class Sprite;
	class SpriteFont;
	class Texture;


	class BatchRenderer
	{
	public:
		BatchRenderer(bool enableSpriteOutline);
		~BatchRenderer() = default;

		void BeginScene(Camera* camera);
		void BeginScene();
		void EndScene();

		void RenderSprite(Sprite* sprite);
		void RenderSprite(Sprite& sprite);

		void RenderSpriteFont(SpriteFont* spriteFont);
		void RenderSpriteFont(SpriteFont& spriteFont);

		void RenderQuad(const Vector2& position, const Vector2& size, const Color& fillColor);
		void RenderQuad(float x, float y, float width, float height, const Color& fillColor);
		void RenderQuad(const Vector2& position, const Vector2& size, const Color& fillColor, const Color& outlineColor, float outlineSize);
		void RenderQuad(float x, float y, float width, float height, const Color& fillColor, const Color& outlineColor, float outlineSize);

		void RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor);
		void RenderQuad(float x, float y, float width, float height, float anchorX, float anchorY, const Color& fillColor);
		void RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize);
		void RenderQuad(float x, float y, float width, float height, float anchorX, float anchorY, const Color& fillColor, const Color& outlineColor, float outlineSize);

		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Color& fillColor);
		void RenderRotatedQuad(float x, float y, float width, float height, float radians, const Color& fillColor);
		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Color& fillColor, const Color& outlineColor, float outlineSize);
		void RenderRotatedQuad(float x, float y, float width, float height, float radians, const Color& fillColor, const Color& outlineColor, float outlineSize);
		
		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor);
		void RenderRotatedQuad(float x, float y, float width, float height, float radians, float anchorX, float anchorY, const Color& fillColor);
		void RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize);
		void RenderRotatedQuad(float x, float y, float width, float height, float radians, float anchorX, float anchorY, const Color& fillColor, const Color& outlineColor, float outlineSize);

		void RenderLine(const Vector2& start, const Vector2& end, const Color& color, float lineWidth);
		void RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth);
		void RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth, const Vector2& position, float radians);

		void RenderPoint(const Vector2& position, const Color& color, float size = 1.0f); //size is the radius of the point

		void RenderCircle(const Vector2& position, float radius, const Color& fillColor); 
		void RenderCircle(float x, float y, float radius, const Color& fillColor);

		void RenderCircle(const Vector2& position, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize); 
		void RenderCircle(float x, float y, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize); 

		unsigned long long GetTotalDrawCalls();
		unsigned long long GetLastFrameDrawCalls();

		void EnableDebugSpriteOutline(bool enable);
		bool IsDebugSpriteOutlineEnabled();

	private:
		friend class Application;

		void SwitchRenderer(RendererType type);
		void NewFrame();

		std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
		std::unique_ptr<LineRenderer> m_LineRenderer;
		std::unique_ptr<PointRenderer> m_PointRenderer;
		std::unique_ptr<CircleRenderer> m_CircleRenderer;
		IRenderer* m_ActiveRenderer;
		Camera* m_SceneCamera;
	};
}
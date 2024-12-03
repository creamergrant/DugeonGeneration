#include "BatchRenderer.h"
#include "Camera.h"
#include "Sprite.h"
#include "SpriteFont.h"
#include "Texture.h"
#include "../Application/Application.h"
#include "../Resources/ResourceManager.h"
#include "../Utils/Text/Text.h"


namespace GameDev2D
{
	BatchRenderer::BatchRenderer(bool enableSpriteOutline) :
		m_SpriteRenderer(nullptr),
		m_LineRenderer(nullptr),
		m_ActiveRenderer(nullptr),
		m_SceneCamera(nullptr)
	{
		m_SpriteRenderer = std::make_unique<SpriteRenderer>();
		m_SpriteRenderer->EnableDebugSpriteOutline(enableSpriteOutline);
		m_LineRenderer = std::make_unique<LineRenderer>();
		m_PointRenderer = std::make_unique<PointRenderer>();
		m_CircleRenderer = std::make_unique<CircleRenderer>();

		SwitchRenderer(RendererType::Sprite);
	}

	void BatchRenderer::BeginScene(Camera* camera)
	{
		m_SceneCamera = camera;

		if (m_ActiveRenderer != nullptr)
		{
			m_ActiveRenderer->BeginScene(m_SceneCamera);
		}
	}

	void BatchRenderer::BeginScene()
	{
		BeginScene(Application::Get().GetGraphics().GetActiveCamera());
	}

	void BatchRenderer::EndScene()
	{
		if (m_ActiveRenderer != nullptr)
		{
			m_ActiveRenderer->EndScene();
		}

		m_SceneCamera = nullptr;
	}

	void BatchRenderer::RenderSprite(Sprite* sprite)
	{
		RenderSprite(*sprite);
	}

	void BatchRenderer::RenderSprite(Sprite& sprite)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}

		m_SpriteRenderer->RenderSprite(sprite);
	}

	void BatchRenderer::RenderSpriteFont(SpriteFont* spriteFont)
	{
		RenderSpriteFont(*spriteFont);
	}

	void BatchRenderer::RenderSpriteFont(SpriteFont& spriteFont)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}

		m_SpriteRenderer->RenderSpriteFont(spriteFont);
	}

	void BatchRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Color& fillColor)
	{
		RenderQuad(position, size, Vector2::Zero, fillColor);
	}

	void BatchRenderer::RenderQuad(float x, float y, float width, float height, const Color& fillColor)
	{
		RenderQuad(Vector2(x, y), Vector2(width, height), fillColor);
	}

	void BatchRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderQuad(position, size, Vector2::Zero, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderQuad(float x, float y, float width, float height, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderQuad(Vector2(x, y), Vector2(width, height), fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}

		m_SpriteRenderer->RenderQuad(position, size, anchor, fillColor);
	}

	void BatchRenderer::RenderQuad(float x, float y, float width, float height, float anchorX, float anchorY, const Color& fillColor)
	{
		RenderQuad(Vector2(x, y), Vector2(width, height), Vector2(anchorX, anchorY), fillColor);
	}

	void BatchRenderer::RenderQuad(const Vector2& position, const Vector2& size, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}

		m_SpriteRenderer->RenderQuad(position, size, anchor, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderQuad(float x, float y, float width, float height, float anchorX, float anchorY, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderQuad(Vector2(x, y), Vector2(width, height), Vector2(anchorX, anchorY), fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Color& fillColor)
	{
		RenderRotatedQuad(position, size, radians, Vector2::Zero, fillColor);
	}

	void BatchRenderer::RenderRotatedQuad(float x, float y, float width, float height, float radians, const Color& fillColor)
	{
		RenderRotatedQuad(Vector2(x, y), Vector2(width, height), radians, fillColor);
	}

	void BatchRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderRotatedQuad(position, size, radians, Vector2::Zero, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderRotatedQuad(float x, float y, float width, float height, float radians, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderRotatedQuad(Vector2(x, y), Vector2(width, height), radians, Vector2::Zero, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}
		m_SpriteRenderer->RenderRotatedQuad(position, size, radians, anchor, fillColor);
	}

	void BatchRenderer::RenderRotatedQuad(float x, float y, float width, float height, float radians, float anchorX, float anchorY, const Color& fillColor)
	{
		RenderRotatedQuad(Vector2(x, y), Vector2(width, height), radians, Vector2(anchorX, anchorY), fillColor);
	}

	void BatchRenderer::RenderRotatedQuad(const Vector2& position, const Vector2& size, float radians, const Vector2& anchor, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Sprite)
		{
			SwitchRenderer(RendererType::Sprite);
		}
		m_SpriteRenderer->RenderRotatedQuad(position, size, radians, anchor, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderRotatedQuad(float x, float y, float width, float height, float radians, float anchorX, float anchorY, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderRotatedQuad(Vector2(x, y), Vector2(width, height), radians, Vector2(anchorX, anchorY), fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderLine(const Vector2& start, const Vector2& end, const Color& color, float lineWidth)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Line)
		{
			SwitchRenderer(RendererType::Line);
		}

		m_LineRenderer->RenderLine(start, end, color, lineWidth);
	}

	void BatchRenderer::RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Line)
		{
			SwitchRenderer(RendererType::Line);
		}

		m_LineRenderer->RenderLineStrip(points, color, lineWidth);
	}

	void BatchRenderer::RenderLineStrip(std::vector<Vector2>& points, const Color& color, float lineWidth, const Vector2& position, float radians)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Line)
		{
			SwitchRenderer(RendererType::Line);
		}

		m_LineRenderer->RenderLineStrip(points, color, lineWidth, position, radians);
	}

	void BatchRenderer::RenderPoint(const Vector2& position, const Color& color, float size)
	{
		if (size == 1.0f)
		{
			if (m_ActiveRenderer->GetType() != RendererType::Point)
			{
				SwitchRenderer(RendererType::Point);
			}
			m_PointRenderer->RenderPoint(position, color);
		}
		else
		{
			RenderCircle(position, size, color);
		}
	}

	void BatchRenderer::RenderCircle(const Vector2& position, float radius, const Color& fillColor)
	{
		RenderCircle(position, radius, fillColor, ColorList::Clear, 0.0f);
	}

	void BatchRenderer::RenderCircle(float x, float y, float radius, const Color& fillColor)
	{
		RenderCircle(Vector2(x, y), radius, fillColor, ColorList::Clear, 0.0f);
	}

	void BatchRenderer::RenderCircle(const Vector2& position, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		if (m_ActiveRenderer->GetType() != RendererType::Circle)
		{
			SwitchRenderer(RendererType::Circle);
		}
		m_CircleRenderer->RenderCircle(position, radius, fillColor, outlineColor, outlineSize);
	}

	void BatchRenderer::RenderCircle(float x, float y, float radius, const Color& fillColor, const Color& outlineColor, float outlineSize)
	{
		RenderCircle(Vector2(x, y), radius, fillColor, outlineColor, outlineSize);
	}

	unsigned long long BatchRenderer::GetTotalDrawCalls()
	{
		return m_SpriteRenderer->GetTotalDrawCalls() + m_LineRenderer->GetTotalDrawCalls() + m_CircleRenderer->GetTotalDrawCalls() + m_PointRenderer->GetTotalDrawCalls();
	}

	unsigned long long BatchRenderer::GetLastFrameDrawCalls()
	{
		return m_SpriteRenderer->GetLastFrameDrawCalls() + m_LineRenderer->GetLastFrameDrawCalls() + m_CircleRenderer->GetLastFrameDrawCalls() + m_PointRenderer->GetLastFrameDrawCalls();
	}

	void BatchRenderer::EnableDebugSpriteOutline(bool enable)
	{
		m_SpriteRenderer->EnableDebugSpriteOutline(enable);
	}

	bool BatchRenderer::IsDebugSpriteOutlineEnabled()
	{
		return m_SpriteRenderer->IsDebugSpriteOutlineEnabled();
	}

	void BatchRenderer::SwitchRenderer(RendererType type)
	{
		if (m_ActiveRenderer != nullptr)
		{
			m_ActiveRenderer->EndScene();
		}

		switch (type)
		{
		case RendererType::Sprite:
			m_ActiveRenderer = m_SpriteRenderer.get();
			break;
		case RendererType::Line:
			m_ActiveRenderer = m_LineRenderer.get();
			break;
		case RendererType::Point:
			m_ActiveRenderer = m_PointRenderer.get();
			break;
		case RendererType::Circle:
			m_ActiveRenderer = m_CircleRenderer.get();
			break;
		default:
			Log::Error(true, LogVerbosity::Graphics, "[Graphics] Unknown renderer selected");
			break;
		}

		if (m_SceneCamera != nullptr)
		{
			m_ActiveRenderer->BeginScene(m_SceneCamera);
		}
	}

	void BatchRenderer::NewFrame()
	{
		m_SpriteRenderer->NewFrame();
		m_LineRenderer->NewFrame();
		m_CircleRenderer->NewFrame();
		m_PointRenderer->NewFrame();
	}
}
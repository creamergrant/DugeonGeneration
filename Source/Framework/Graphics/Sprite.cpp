#include "Sprite.h"
#include "../Application/Application.h"
#include <GameDev2D.h>
#include <vector>


namespace GameDev2D
{
    Sprite::Sprite(Texture* aTexture) : Transformable(),
        m_Texture(nullptr),
        m_Frame(Rect())
    {
		SetTexture(aTexture);
		Application::Get().GetResourceManager().GetTextureCache().ResourceUnloadedSlot.connect<&Sprite::OnTextureUnloaded>(this);
    }
    
	Sprite::Sprite(const std::string& filename) : Transformable(),
		m_Texture(nullptr),
		m_Frame(Rect())
	{
		SetTexture(Application::Get().GetResourceManager().GetTextureCache().Get(filename));
		Application::Get().GetResourceManager().GetTextureCache().ResourceUnloadedSlot.connect<&Sprite::OnTextureUnloaded>(this);
	}

	Sprite::Sprite() : Transformable(),
		m_Texture(nullptr),
		m_Frame(Rect())
	{
		SetTexture(Application::Get().GetResourceManager().GetTextureCache().GetDefaultResource());
		Application::Get().GetResourceManager().GetTextureCache().ResourceUnloadedSlot.connect<&Sprite::OnTextureUnloaded>(this);
	}

    Sprite::~Sprite()
    {
		Application::Get().GetResourceManager().GetTextureCache().ResourceUnloadedSlot.disconnect<&Sprite::OnTextureUnloaded>(this);
    }

    float Sprite::GetWidth()
    {
        return m_Frame.size.x;
    }

	float Sprite::GetHeight()
	{
		return m_Frame.size.y;
	}

	Vector2 Sprite::GetSize()
	{
		return Vector2(GetWidth(), GetHeight());
	}

	Color Sprite::GetColor()
	{
		return m_Color;
	}

	void Sprite::SetColor(Color aColor)
	{
		m_Color = aColor;
	}

	float Sprite::GetAlpha()
	{
		return m_Color.a;
	}

	void Sprite::SetAlpha(float aAlpha)
	{
		Color color = m_Color;
		color.a = aAlpha;
		SetColor(color);
	}

	Vector2 Sprite::GetAnchor()
	{
		return m_Anchor;
	}

	void Sprite::SetAnchor(Vector2 aAnchor)
	{
		m_Anchor = aAnchor;
	}

	void Sprite::SetAnchor(float aAnchorX, float aAnchorY)
	{
		m_Anchor.x = aAnchorX;
		m_Anchor.y = aAnchorY;
	}

	void Sprite::SetAnchorX(float aAnchorX)
	{
		m_Anchor.x = aAnchorX;
	}

	void Sprite::SetAnchorY(float aAnchorY)
	{
		m_Anchor.y = aAnchorY;
	}

	float Sprite::GetLeftEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[LeftEdge];
	}

	float Sprite::GetRightEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[RightEdge];
	}

	float Sprite::GetTopEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[TopEdge];
	}

	float Sprite::GetBottomEdge()
	{
		if (m_EdgesCalculationDirty == true || m_Parent != nullptr)
		{
			CalculateEdges();
		}

		return m_Edges[BottomEdge];
	}

	void Sprite::SetFrame(Rect aFrame)
	{
		//Set the source frame
		m_Frame.origin = aFrame.origin;
		m_Frame.size = aFrame.size;


	}

    Rect Sprite::GetFrame()
    {
        return m_Frame;
    }

	void Sprite::GetUVs(Vector2 uvs[4])
	{
		float u1 = (float)m_Frame.origin.x / (float)m_Texture->GetWidth();
		float u2 = (float)(m_Frame.origin.x + m_Frame.size.x) / (float)m_Texture->GetWidth();
		float v1 = 1.0f - (((float)m_Frame.origin.y + m_Frame.size.y) / (float)m_Texture->GetHeight());
		float v2 = 1.0f - (m_Frame.origin.y / (float)m_Texture->GetHeight());

		uvs[0] = Vector2(u1, v1);
		uvs[1] = Vector2(u2, v1);
		uvs[2] = Vector2(u2, v2);
		uvs[3] = Vector2(u1, v2);
	}

    void Sprite::SetTexture(Texture* aTexture)
    {
        m_Texture = aTexture;
		SetFrame(Rect(Vector2(0.0f, 0.0f), Vector2((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight())));
    }

	void Sprite::SetTexture(std::string filename)
	{
		SetTexture(Application::Get().GetResourceManager().GetTextureCache().Get(filename));
	}
        
    Texture* Sprite::GetTexture() const
    {
        return m_Texture;
    }

	void Sprite::LocalTransformIsDirty()
	{
		m_EdgesCalculationDirty = true;
		Transformable::LocalTransformIsDirty();
	}

	void Sprite::CalculateEdges()
	{
		Vector2 position = GetWorldPosition();
		float radians = GetWorldRadians();
		float w = GetWidth();
		float h = GetHeight();
		float c = cosf(radians);
		float s = sinf(radians);
		float cw = c * w;
		float ch = c * h;
		float sw = s * w;
		float sh = s * h;
		float ex = (fabsf(sh) + fabsf(cw)) * 0.5f;
		float ey = (fabsf(sw) + fabsf(ch)) * 0.5f;
		float ax = GetAnchor().x;
		float ay = GetAnchor().y;

		//Calculate the edge positions
		m_Edges[LeftEdge] = position.x + sh * (ay - 0.5f) - cw * (ax - 0.5f) - ex;
		m_Edges[RightEdge] = position.x + sh * (ay - 0.5f) - cw * (ax - 0.5f) + ex;
		m_Edges[TopEdge] = position.y - sw * (ax - 0.5f) - ch * (ay - 0.5f) + ey;
		m_Edges[BottomEdge] = position.y - sw * (ax - 0.5f) - ch * (ay - 0.5f) - ey;
		m_EdgesCalculationDirty = false;
	}

	void Sprite::OnTextureUnloaded(Texture* texture)
	{
		if (texture == m_Texture)
		{
			//The texture the Sprite was using was just unloaded, replace it with the default texture
			Texture* defaultTexture = Application::Get().GetResourceManager().GetTextureCache().GetDefaultResource();
			SetTexture(defaultTexture);
		}
	}
}
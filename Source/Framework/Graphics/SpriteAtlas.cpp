#include "SpriteAtlas.h"
#include "../Application/Application.h"
#include <json.h>
#include <fstream>
#include <assert.h>


namespace GameDev2D
{
    SpriteAtlas::SpriteAtlas(const std::string& filename) : Sprite(Application::Get().GetResourceManager().GetTextureCache().Get(filename)),
        m_Filename(filename)
    {}

    SpriteAtlas::SpriteAtlas() : Sprite(Application::Get().GetResourceManager().GetTextureCache().GetDefaultResource()),
        m_Filename("")
    {}

    SpriteAtlas::~SpriteAtlas()
    {}

    void SpriteAtlas::SetTexture(std::string filename)
    {
        m_Filename = filename;
        Sprite::SetTexture(m_Filename);
    }

    void SpriteAtlas::UseFrame(const std::string& atlasKey)
    {
        Rect frame = Application::Get().GetResourceManager().GetAtlasCache().Get(m_Filename)->Frame(atlasKey);
		if (frame.origin == Vector2::Zero && frame.size == Vector2::Zero)
		{
            Texture* defaultTexture = Application::Get().GetResourceManager().GetTextureCache().GetDefaultResource();
            Sprite::SetTexture(defaultTexture);
		}
		else
		{
			SetFrame(frame);
		}
    }
}
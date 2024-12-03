#pragma once

#include "Sprite.h"
#include <string>


namespace GameDev2D
{
    class AtlasMap;

    class SpriteAtlas : public Sprite
    {
    public: 
        //Creates a SpriteAtlas for the given filename, all the frames in the atlas will 
        //be stored and can be set using the UseFrame method(). Make sure to load the Texture 
        //that the atlas uses ahead of time
        SpriteAtlas(const std::string& filename);
		SpriteAtlas();
        ~SpriteAtlas();

		void SetTexture(std::string filename) override;

        //Sets the specific frame for the atlasKey
        void UseFrame(const std::string& atlasKey);

    private:
        //Member variables
        std::string m_Filename;
    };
}

#pragma once

#include "GraphicTypes.h"


namespace GameDev2D
{
    class Texture
    {
    public:
        //Creates a Texture object using the image data
        Texture(const ImageData& imageData);
        Texture() = delete;

        //Prevent Texture from being copied
        Texture(Texture& aTexture) = delete;
        Texture& operator=(const Texture&) = delete;
        
        ~Texture();

        //Returns the width and height of the texture
        unsigned int GetWidth();
        unsigned int GetHeight();
        
        //Returns the id of the texture
        unsigned int GetId();

        //Returns the Texture's PixelFormat
        PixelFormat GetPixelFormat();
        
        //Sets the wrap mode of the Sprite
        void SetWrap(WrapMode wrapModeS, WrapMode wrapModeT);
        void SetWrap(Wrap wrap);

        //Returns the wrap mode of the Sprite
        Wrap GetWrap();

        //Set (or update) the contents of the Texture
        void SetData(const ImageData& imageData);

        //Bind the texture to the slot
        void Bind(unsigned int slot = 0);

        bool operator==(const Texture& texture) const;

    private:
        //Member variables
        PixelFormat m_Format;
        Wrap m_Wrap;
        unsigned int m_Width;
        unsigned int m_Height;
        unsigned int m_ID;
    };
}

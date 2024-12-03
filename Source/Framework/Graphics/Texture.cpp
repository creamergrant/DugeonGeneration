#include "Texture.h"
#include "../Application/Application.h"


namespace GameDev2D
{
    Texture::Texture(const ImageData& imageData) :
        m_Format(imageData.format),
        m_Wrap(WrapMode::ClampToEdge, WrapMode::ClampToEdge),
        m_Width(imageData.width),
        m_Height(imageData.height),
        m_ID(0)
    {
        Application::Get().GetGraphics().CreateTexture(&m_ID);
        Application::Get().GetGraphics().SetTextureStorage(m_ID, m_Width, m_Height, m_Format);
        Application::Get().GetGraphics().SetTextureFilter(m_ID, TextureMinFilter::Linear, TextureMagFilter::Nearest);
        Application::Get().GetGraphics().SetTextureWrap(m_ID, m_Wrap);

        if (imageData.data.size() > 0)
        {
            Application::Get().GetGraphics().SetTextureData(m_ID, imageData);
        }
    }

    Texture::~Texture()
    {
        Application::Get().GetGraphics().DeleteTexture(&m_ID);
    }

    unsigned int Texture::GetWidth()
    {
        return m_Width;
    }

    unsigned int Texture::GetHeight()
    {
        return m_Height;
    }

    unsigned int Texture::GetId()
    {
        return m_ID;
    }

    PixelFormat Texture::GetPixelFormat()
    {
        return m_Format;
    }

    void Texture::SetWrap(WrapMode wrapModeS, WrapMode wrapModeT)
    {
        m_Wrap.s = wrapModeS;
        m_Wrap.t = wrapModeT;
        Application::Get().GetGraphics().SetTextureWrap(m_ID, m_Wrap);
    }

    void Texture::SetWrap(Wrap wrap)
    {
        SetWrap(wrap.s, wrap.t);
    }

    Wrap Texture::GetWrap()
    {
        return m_Wrap;
    }

    void Texture::SetData(const ImageData& imageData)
    {
        //The Texture id can't be zero
        assert(m_ID != 0);

        m_Width = imageData.width;
        m_Height = imageData.height;
        m_Format = imageData.format;

        Application::Get().GetGraphics().SetTextureStorage(m_ID, m_Width, m_Height, m_Format);
        Application::Get().GetGraphics().SetTextureFilter(m_ID, TextureMinFilter::Linear, TextureMagFilter::Nearest);
        Application::Get().GetGraphics().SetTextureData(m_ID, imageData);
    }

    void Texture::Bind(unsigned int slot)
    {
        Application::Get().GetGraphics().BindTextureUnit(m_ID, slot);
    }

    bool Texture::operator==(const Texture& texture) const
    {
        return m_ID == texture.m_ID;
    }
}

#include "GameDev2D.h"


namespace GameDev2D
{
    unsigned int GetScreenWidth()
    {
        return Application::Get().GetWindow().GetWidth();
    }

    unsigned int GetScreenHeight()
    {
        return Application::Get().GetWindow().GetHeight();
    }

    unsigned int GetHalfScreenWidth()
    {
        return Application::Get().GetWindow().GetWidth() / 2;
    }

    unsigned int GetHalfScreenHeight()
    {
        return Application::Get().GetWindow().GetHeight() / 2;
    }

    float GetTextureWidth(const std::string& filename)
    {
        float width = 0.0f;
        Texture* texture = Application::Get().GetResourceManager().GetTextureCache().Get(filename);
        if (texture != nullptr)
        {
            width = static_cast<float>(texture->GetWidth());
        }

        return width;
    }

    float GetTextureHeight(const std::string& filename)
    {
        float height = 0.0f;
        Texture* texture = Application::Get().GetResourceManager().GetTextureCache().Get(filename);
        if (texture != nullptr)
        {
            height = static_cast<float>(texture->GetHeight());
        }

        return height;
    }

    Camera& GetMainCamera()
    {
        return *(Application::Get().GetGraphics().GetMainCamera());
    }

	void LoadAudio(const std::string& filename)
	{
        Resource resource(filename, "wav", "Audio");
        Application::Get().GetResourceManager().GetAudioCache().Load(resource);
	}

	void UnloadAudio(const std::string& filename)
	{
        Application::Get().GetResourceManager().GetAudioCache().Unload(filename);
	}

    Texture* LoadTexture(const std::string& filename)
    {
        Resource resource(filename, "png", "Images");
        Application::Get().GetResourceManager().GetTextureCache().Load(resource);
        return Application::Get().GetResourceManager().GetTextureCache().Get(filename);
    }

    void UnloadTexture(const std::string& identifier)
    {
        Application::Get().GetResourceManager().GetTextureCache().Unload(identifier);
    }

    void UnloadTexture(Texture* texture)
    {
        std::string identifier = Application::Get().GetResourceManager().GetTextureCache().GetKey(texture);
        UnloadTexture(identifier);
    }

	void LoadFont(const std::string& filename)
	{
        Resource resource(filename, "png", "Fonts");
        Application::Get().GetResourceManager().GetFontCache().Load(resource);
	}

	void UnloadFont(const std::string& identifier)
	{
        Application::Get().GetResourceManager().GetFontCache().Unload(identifier);
	}

    bool IsKeyUp(KeyCode aKeyCode)
    {
        return Application::Get().GetInputManager().GetKeyboard().GetState(aKeyCode).keyState == KeyState::Up;
    }

    bool IsKeyDown(KeyCode aKeyCode)
    {
        Keyboard::State keyboardState = Application::Get().GetInputManager().GetKeyboard().GetState(aKeyCode);
        return keyboardState.keyState == KeyState::Down || keyboardState.keyState == KeyState::Repeat;
    }

    bool IsMouseButtonUp(MouseButton aButton)
    {
        return !Application::Get().GetInputManager().GetMouse().IsButtonPressed(aButton);
    }

    bool IsMouseButtonDown(MouseButton aButton)
    {
        return Application::Get().GetInputManager().GetMouse().IsButtonPressed(aButton);
    }

    Vector2 GetMousePosition()
    {
        return Application::Get().GetInputManager().GetMouse().GetPosition();
    }
}
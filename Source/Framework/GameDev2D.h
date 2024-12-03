#pragma once


#include "Application/Application.h"
#include "Application/Config.h"
#include "Application/GameLoop.h"
#include "Application/TimeStep.h"
#include "Audio/Audio.h"
#include "Audio/AudioTypes.h"
#include "Audio/AudioEngine.h"
#include "Debug/Log.h"
#include "Debug/Profile.h"
#include "Debug/DebugUI.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/BatchRenderer.h"
#include "Graphics/Buffer.h"
#include "Graphics/Camera.h"
#include "Graphics/CircleRenderer.h"
#include "Graphics/Color.h"
#include "Graphics/ColorList.h"
#include "Graphics/Font.h"
#include "Graphics/Graphics.h"
#include "Graphics/GraphicTypes.h"
#include "Graphics/LineRenderer.h"
#include "Graphics/PointRenderer.h"
#include "Graphics/Shader.h"
#include "Graphics/Sprite.h"
#include "Graphics/SpriteAtlas.h"
#include "Graphics/SpriteFont.h"
#include "Graphics/SpriteRenderer.h"
#include "Graphics/Texture.h"
#include "Graphics/VertexArray.h"
#include "Input/Gamepad.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/InputManager.h"
#include "Math/AABB.h"
#include "Math/LineSegment.h"
#include "Math/Math.h"
#include "Math/Matrix.h"
#include "Math/Random.h"
#include "Math/Transformable.h"
#include "Math/Vector2.h"
#include "Memory/MemoryBlock.h"
#include "Memory/ObjectPool.h"
#include "Platform/GameWindow.h"
#include "Platform/Platform.h"
#include "Platform/Windows/FileSystem.h"
#include "Platform/Windows/HardwareInfo.h"
#include "Resources/Resource.h"
#include "Resources/ResourceCache.h"
#include "Resources/ResourceManager.h"
#include "Utils/Png/Png.h"
#include "Utils/Text/Text.h"
#include "Utils/Wave/Wave.h"
#include "crtdbg.h"
#include <functional>

#define CHECK_FOR_MEMORY_LEAKS 1

//Used to track down memory leaks
#ifdef _DEBUG
#if CHECK_FOR_MEMORY_LEAKS
#define new   new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#endif 


#define SafeDelete(p)  do{ if(p != nullptr) {delete p; p = nullptr;} }while(0)
#define SafeDeleteArray(p)   do{if(p != nullptr){delete[] p; p = nullptr;} }while(0)

namespace GameDev2D
{
    // Returns the width of the Window
    unsigned int GetScreenWidth();


    // Returns the height of the Window
    unsigned int GetScreenHeight();


    // Returns half of the width of the Window
    unsigned int GetHalfScreenWidth();


    // Returns half of the height of the Window
    unsigned int GetHalfScreenHeight();


    // Returns the width of an already loaded Texture
    float GetTextureWidth(const std::string& filename);


    // Returns the width of an already loaded Texture
    float GetTextureHeight(const std::string& filename);


    // Returns the main Camera
    Camera& GetMainCamera();


	// Loads an audio file. All audio files MUST be of type wav. If the audio file doesn't exist, then a default tone will be 
	// loaded instead
	void LoadAudio(const std::string& filename);


	// Unloads an already loaded audio file. You must make sure to unload all audio files that you load, if you don't then you 
	// aren't properly releasing precious memory back to the operating system
	void UnloadAudio(const std::string& filename);


    // Loads a Texture from a file. All Texture files MUST be of type png. If a Texture file doesn't exist a default 'checkboard'
    // Texture will be loaded in its place.
    Texture* LoadTexture(const std::string& filename);


    // Unloads an already loaded Texture. You must make sure to unload any Texture that you load, if you don't you aren't properly 
    // releasing precious memory back to the operating system
    void UnloadTexture(const std::string& filename);


    // Unloads an already loaded Texture. You must make sure to unload any Texture that you load, if you don't you aren't properly 
    // releasing precious memory back to the operating system
    void UnloadTexture(Texture* texture);


	// Loads a Font from a file. Font files can be of type ttf or otf. If a Font file doesn't exist a default 'font'
	// will be loaded in its place.
	void LoadFont(const std::string& filename);


	// Unloads an already loaded Font. You must make sure to unload any Font that you load, if you don't you are properly 
	// releasing precious memory back to the operating system
	void UnloadFont(const std::string& filename);


    // Returns wether a keyboard key is Up or not
    bool IsKeyUp(KeyCode key);


    // Returns wether a keyboard key is Down or not
    bool IsKeyDown(KeyCode key);


    // Returns wether the mouse button is Up or not
    bool IsMouseButtonUp(MouseButton button);


    // Returns wether the mouse button is Down or not
    bool IsMouseButtonDown(MouseButton button);


    // Returns the current position of the mouse
    Vector2 GetMousePosition();
}
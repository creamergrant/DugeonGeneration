#pragma once

#include "../Application/Config.h"
#include <entt.hpp>
#include <string>

struct GLFWwindow;

namespace GameDev2D
{
    const unsigned int OPENGL_VERSION_MAJOR = 4;
    const unsigned int OPENGL_VERSION_MINOR = 5;

    class GameWindowCallback
    {
    public:
        virtual ~GameWindowCallback() {}
        virtual void OnWindowResized(unsigned int, unsigned int) = 0;
        virtual void OnWindowClosed() = 0;
        virtual void OnWindowFocusChanged(bool isFocused) = 0;
    };

    //The GameWindow handle window creation, event handling and OpenGL context creation
    class GameWindow
    {
    public:
        GameWindow(GameWindowCallback* callback, const Config& config);
        ~GameWindow();

        //Returns the Window's current width and height
        unsigned int GetWidth();
        unsigned int GetHeight();

        //Resizes the Window    
        void Resize(unsigned int width, unsigned int height);

        //Set's the title of the window
        void SetTitle(const std::string& title);

        //Fullscreen methods
        void SetFullscreen(bool isFullscreen);
        bool IsFullscreen();

        //Vertical sync methods
        void EnableVerticalSync(bool isEnabled);
        bool IsVerticalSyncEnabled();

    private:
        //Allow the Application class to access non-public members
        friend class Application;

        void SwapDrawBuffer();
        void PollEvents();

        //Member variables
        GameWindowCallback* m_Callback;
        GLFWwindow* m_Window;
        unsigned int m_Width;
        unsigned int m_Height;
        bool m_IsFullscreen;
        bool m_IsVSyncEnabled;
    };
}

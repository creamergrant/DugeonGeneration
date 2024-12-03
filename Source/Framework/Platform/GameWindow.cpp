    
#include "GameWindow.h"
#include "../Application/Application.h"
#include "../Input/InputManager.h"
#include "../Utils/Png/Png.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace GameDev2D
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        Log::Message("GLFW Error(%i) %s", error, description);
    }

    GameWindow::GameWindow(GameWindowCallback* callback, const Config& config) :
        m_Callback(callback),
        m_Window(nullptr),
        m_Width(config.window.width),
        m_Height(config.window.height),
        m_IsFullscreen(config.window.fullscreen),
        m_IsVSyncEnabled(config.window.vsync)
    {
        //Initialize glfw
        int success = glfwInit();
        assert(success);

        glfwSetErrorCallback(GLFWErrorCallback);

        //Set the OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);

        //
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        //Create the window
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        m_Window = glfwCreateWindow(static_cast<int>(m_Width), static_cast<int>(m_Height), config.window.title.c_str(), m_IsFullscreen ? monitor : nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        assert(status);

        if (!m_IsFullscreen)
        {
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            int width = static_cast<int>(m_Width);
            int height = static_cast<int>(m_Height);
            int x = (mode->width - width) / 2;
            int y = (mode->height - height) / 2;
            glfwSetWindowPos(m_Window, x, y);
        }

        //Set the windows icon
        ImageData imageData;
        std::string path = Application::Get().GetFileSystem().GetPathForWindowsIcon();
        if (Application::Get().GetFileSystem().DoesFileExistAtPath(path))
        {
            Png::LoadFromPath(path, imageData, false);
            GLFWimage image = { static_cast<int>(imageData.width), static_cast<int>(imageData.height), &imageData.data[0] };
            glfwSetWindowIcon(m_Window, 1, &image);
        }

        glfwSwapInterval(static_cast<int>(m_IsVSyncEnabled));
        glfwSetWindowUserPointer(m_Window, this);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
                if (static_cast<unsigned int>(width) != gameWindow->m_Width || static_cast<unsigned int>(height) != gameWindow->m_Height)
                {
                    gameWindow->m_Width = static_cast<unsigned int>(width);
                    gameWindow->m_Height = static_cast<unsigned int>(height);
                    if (gameWindow->m_Callback != nullptr)
                    {
                        gameWindow->m_Callback->OnWindowResized(gameWindow->m_Width, gameWindow->m_Height);
                    }
                }
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
                if (gameWindow->m_Callback != nullptr)
                {
                    gameWindow->m_Callback->OnWindowClosed();
                }
            });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focus)
            {
                GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
                if (gameWindow->m_Callback != nullptr)
                {
                    gameWindow->m_Callback->OnWindowFocusChanged(focus == 1);
                }
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));

                switch (action)
                {
                case GLFW_PRESS:
                {
                    if (gameWindow->IsFullscreen() && static_cast<KeyCode>(key) == KeyCode::Escape)
                    {
                        gameWindow->SetFullscreen(false);
                    }

                    Application::Get().GetInputManager().GetKeyboard().HandleKeyEvent(static_cast<KeyCode>(key), KeyState::Down);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Application::Get().GetInputManager().GetKeyboard().HandleKeyEvent(static_cast<KeyCode>(key), KeyState::Up);
                    break;
                }
                case GLFW_REPEAT:
                {
                    Application::Get().GetInputManager().GetKeyboard().HandleKeyEvent(static_cast<KeyCode>(key), KeyState::Repeat);
                    break;
                }
                }
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    Application::Get().GetInputManager().GetMouse().HandleButtonEvent(static_cast<MouseButton>(button), MouseButtonState::Pressed);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Application::Get().GetInputManager().GetMouse().HandleButtonEvent(static_cast<MouseButton>(button), MouseButtonState::Released);
                    break;
                }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                Application::Get().GetInputManager().GetMouse().HandleScroll(static_cast<float>(xOffset), static_cast<float>(yOffset));
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x, double y)
            {
                GameWindow* gameWindow = static_cast<GameWindow*>(glfwGetWindowUserPointer(window));
                Application::Get().GetInputManager().GetMouse().HandleMouseMoved(static_cast<float>(x), static_cast<float>(gameWindow->GetHeight()) - static_cast<float>(y));
            });

        glfwSetJoystickCallback([](int jid, int event)
            {
                if (event == GLFW_CONNECTED)
                {
                    if (glfwJoystickIsGamepad(jid) == GLFW_TRUE)
                    {
                        GamepadPort port = static_cast<GamepadPort>(jid);
                        Application::Get().GetInputManager().GetGamepad(port).OnConnected();
                    }
                }
                else if (event == GLFW_DISCONNECTED)
                {
                    GamepadPort port = static_cast<GamepadPort>(jid);
                    Application::Get().GetInputManager().GetGamepad(port).OnDisconnected();
                }
            });
    }

    GameWindow::~GameWindow()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    unsigned int GameWindow::GetWidth()
    {
        return m_Width;
    }

    unsigned int GameWindow::GetHeight()
    {
        return m_Height;
    }

    void GameWindow::Resize(unsigned int width, unsigned int height)
    {
        glfwSetWindowSize(m_Window, static_cast<int>(width), static_cast<int>(height));
    }

    void GameWindow::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_Window, title.c_str());
    }

    void GameWindow::SetFullscreen(bool isFullscreen)
    {
        m_IsFullscreen = isFullscreen;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        int width = static_cast<int>(m_Width);
        int height = static_cast<int>(m_Height);

        glfwSetWindowMonitor(m_Window, m_IsFullscreen ? monitor : nullptr, 0, 0, width, height, mode->refreshRate);

        if (!m_IsFullscreen)
        {
            mode = glfwGetVideoMode(monitor);
            int x = (mode->width - width) / 2;
            int y = (mode->height - height) / 2;
            glfwSetWindowPos(m_Window, x, y);
        }
    }

    bool GameWindow::IsFullscreen()
    {
        return m_IsFullscreen;
    }

    void GameWindow::EnableVerticalSync(bool isEnabled)
    {
        m_IsVSyncEnabled = isEnabled;
        glfwSwapInterval(static_cast<int>(isEnabled));
    }

    bool GameWindow::IsVerticalSyncEnabled()
    {
        return m_IsVSyncEnabled;
    }

    void GameWindow::SwapDrawBuffer()
    {
        glfwSwapBuffers(m_Window);
    }

    void GameWindow::PollEvents()
    {
        glfwPollEvents();
    }
}
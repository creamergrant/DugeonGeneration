#include "Application.h"
#include "Config.h"
#include "../Debug/Log.h"
#include "../Graphics/BatchRenderer.h"


namespace GameDev2D
{
    const Color Application::kSplashBackgroundColor(0x292929ff);
    const double Application::kSplashDuration = 3.0;
    const double Application::kFadeDuration = 0.5;


    Application* Application::s_Instance = nullptr;

    Application::Application() :
        UpdateSlot(m_UpdateSignal),
        RenderSlot(m_RenderSignal),
        LateRenderSlot(m_LateRenderSignal),
        WindowResizedSlot(m_WindowResizedSignal),
        WindowFocusChangedSlot(m_WindowFocusChangedSignal),
        WindowClosedSlot(m_WindowClosedSignal),
        m_GameWindow(nullptr),
        m_GameLoop(nullptr),
        m_ResourceManager(nullptr),
        m_AudioEngine(nullptr),
        m_Graphics(nullptr),
        m_InputManager(nullptr),
        m_DebugUI(nullptr),
        m_FileSystem(),
        m_HardwareInfo(),
        m_BatchRenderer(nullptr),
        m_State(State::Splash),
        m_SplashElapsed(0.0),
        m_FadeElapsed(0.0),
        m_FadeAlpha(0.0f),
        m_IsSuspended(false),
        m_IsRunning(true)
    {
        //Set the static instance variable
        s_Instance = this;

        //Load the config file
        Config config;
        config.Load();

        //Create the Game window
        m_GameWindow = std::make_unique<GameWindow>(this, config);

        //Create the Graphics instance
        m_Graphics = std::make_unique<Graphics>();
        m_Graphics->SetClearColor(config.renderer.clearColor);

        //Create the rest of the Application's services
        m_GameLoop = std::make_unique<GameLoop>(this);
        m_InputManager = std::make_unique<InputManager>();
        m_AudioEngine = std::make_unique<AudioEngine>();
        m_ResourceManager = std::make_unique<ResourceManager>(config.resources.autoLoad);
        m_BatchRenderer = std::make_unique<BatchRenderer>(config.debug.drawSpriteOutline);
        m_DebugUI = std::make_unique<DebugUI>();

        if (config.debug.drawFps)
            m_DebugUI->UnsignedIntSlot.connect<&GameLoop::GetFPS>(m_GameLoop);
#if DEBUG
        if (config.debug.drawElapsed)
            m_DebugUI->DoubleSlot.connect<&GameLoop::GetElapsed>(m_GameLoop);
        if (config.debug.drawDelta)
            m_DebugUI->DoubleSlot.connect<&GameLoop::GetDelta>(m_GameLoop);
        if (config.debug.drawTotalDrawCalls)
            m_DebugUI->UnsignedLongLongSlot.connect<&BatchRenderer::GetTotalDrawCalls>(m_BatchRenderer);
        if (config.debug.drawFrameDrawCalls)
            m_DebugUI->UnsignedLongLongSlot.connect<&BatchRenderer::GetLastFrameDrawCalls>(m_BatchRenderer);
        if (config.debug.drawCameraPosition)
            m_DebugUI->Vector2Slot.connect<&Camera::GetPosition>(m_Graphics->GetMainCamera());
#endif

        if (m_ResourceManager->GetTextureCache().IsLoaded("SplashLogo") && !config.application.disableSplash)
        {
            float windowWidth = static_cast<float>(m_GameWindow->GetWidth());
            float windowHeight = static_cast<float>(m_GameWindow->GetHeight());

            m_SplashLogo = std::make_unique<Sprite>("SplashLogo");
            m_SplashLogo->SetAnchor(0.5f, 0.5f);
            m_SplashLogo->SetPosition(Vector2(windowWidth * 0.5f, windowHeight * 0.5f));

            m_BatchRenderer->BeginScene();
            m_BatchRenderer->RenderQuad(Vector2::Zero, Vector2(windowWidth, windowHeight), kSplashBackgroundColor);
            m_BatchRenderer->RenderSprite(m_SplashLogo.get());
            m_BatchRenderer->EndScene();

            m_GameWindow->SwapDrawBuffer();
        }
        else
        {
            m_State = State::Game;
        }

        //Initialize the log file used by the Debug Log class
        Log::Init();

        //Log the platform details (CPU, RAM, GPU and hardware info)
        m_HardwareInfo.Log();
    }

    Application& Application::Get()
    {
        return *s_Instance;
    }

    FileSystem& Application::GetFileSystem()
    {
        return m_FileSystem;
    }

    GameWindow& Application::GetWindow()
    {
        return *m_GameWindow;
    }

    GameLoop& Application::GetGameLoop()
    {
        return *m_GameLoop;
    }

    ResourceManager& Application::GetResourceManager()
    {
        return *m_ResourceManager;
    }

    AudioEngine& Application::GetAudioEngine()
    {
        return *m_AudioEngine;
    }

    Graphics& Application::GetGraphics()
    {
        return *m_Graphics;
    }

    InputManager& Application::GetInputManager()
    {
        return *m_InputManager;
    }

    DebugUI& Application::GetDebugUI()
    {
        return *m_DebugUI;
    }

    void Application::Run()
    {
        do
        {
            m_GameWindow->PollEvents();

            if (m_IsSuspended == false)
            {
                m_GameLoop->Step();
            }
        } while (m_IsRunning);
    }

    void Application::OnUpdate(float delta)
    {
        if (m_IsRunning == true)
        {
            if (m_State == State::Splash)
            {
                m_SplashElapsed += delta;
                if (m_SplashElapsed >= kSplashDuration)
                {
                    m_State = State::FadeOut;
                }
            }
            else if (m_State == State::FadeOut)
            {
                m_FadeElapsed += delta;
                m_FadeAlpha = static_cast<float>(m_FadeElapsed / kFadeDuration);

                if (m_FadeElapsed >= kFadeDuration)
                {
                    m_FadeElapsed = 0.0f;
                    m_State = State::FadeIn;
                }
            }
            else if (m_State == State::FadeIn)
            {
                m_FadeElapsed += delta;
                m_FadeAlpha = 1.0f - static_cast<float>(m_FadeElapsed / kFadeDuration);

                if (m_FadeElapsed >= kFadeDuration)
                {
                    m_State = State::Game;
                }
            }
            else
            {
                m_UpdateSignal.publish(delta);
            }
        }
    }

    void Application::OnDraw()
    {
        if (m_IsRunning == true && (m_State == State::Game || m_State == State::FadeOut || m_State == State::FadeIn))
        {
            if (m_IsSuspended == false)
            {
                m_BatchRenderer->NewFrame();
                m_Graphics->Clear();
            }

            if (m_State == State::Game || m_State == State::FadeIn)
            {
                m_RenderSignal.publish(*m_BatchRenderer);
                m_LateRenderSignal.publish(*m_BatchRenderer);

                if (m_State == State::FadeIn)
                {
                    Color color = ColorList::Black;
                    color.a = m_FadeAlpha;

                    m_BatchRenderer->BeginScene();
                    m_BatchRenderer->RenderQuad(Vector2::Zero, Vector2(static_cast<float>(m_GameWindow->GetWidth()), static_cast<float>(m_GameWindow->GetHeight())), color);
                    m_BatchRenderer->EndScene();
                }
            }
            else if (m_State == State::FadeOut)
            {
                float windowWidth = static_cast<float>(m_GameWindow->GetWidth());
                float windowHeight = static_cast<float>(m_GameWindow->GetHeight());

                Color color = ColorList::Black;
                color.a = m_FadeAlpha;

                m_BatchRenderer->BeginScene();
                m_BatchRenderer->RenderQuad(Vector2::Zero, Vector2(windowWidth, windowHeight), kSplashBackgroundColor);
                m_BatchRenderer->RenderSprite(m_SplashLogo.get());
                m_BatchRenderer->RenderQuad(Vector2::Zero, Vector2(windowWidth, windowHeight), color);
                m_BatchRenderer->EndScene();
            }

            if (m_IsSuspended == false)
            {
                if (m_GameWindow != nullptr)
                {
                    m_GameWindow->SwapDrawBuffer();
                }
            }
        }
    }

    bool Application::IsSuspended()
    {
        return m_IsSuspended;
    }

    void Application::OnWindowResized(unsigned int width, unsigned int height)
    {
        if (width == 0 || height == 0)
        {
            m_IsSuspended = true;
            return;
        }

        m_IsSuspended = false;

        Log::Message(LogVerbosity::Application, "[Application] window resized: (%u, %u)", width, height);
        m_WindowResizedSignal.publish(width, height);
    }

    void Application::OnWindowClosed()
    {
        if (m_IsRunning == true)
        {
            m_IsRunning = false;
            Log::Message(LogVerbosity::Application, "[Application] window closed");
            m_WindowClosedSignal.publish();
        }
    }

    void Application::OnWindowFocusChanged(bool isFocused)
    {
        m_IsSuspended = !isFocused;
        Log::Message(LogVerbosity::Application, "[Application] window %s", isFocused ? "gained focus" : "lost focus");
        m_WindowFocusChangedSignal.publish(isFocused);
    }
}
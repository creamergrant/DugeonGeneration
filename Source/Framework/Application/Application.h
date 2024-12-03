#pragma once

#include "GameLoop.h"
#include "../Platform/GameWindow.h"
#include "../Platform/Windows/FileSystem.h"
#include "../Platform/Windows/HardwareInfo.h"
#include "../Resources/ResourceManager.h"
#include "../Audio/AudioEngine.h"
#include "../Graphics/Graphics.h"
#include "../Graphics/Sprite.h"
#include "../Input/InputManager.h"
#include "../Debug/DebugUI.h"
#include <entt.hpp>
#include <functional>
#include <string>

int main(int argc, char** argv);

namespace GameDev2D
{
    class BatchRenderer;

    class Application : public GameLoopCallback, public GameWindowCallback
    {
    public:
        //The constructor takes in the Window the application is in, the window title and the target frame rate.
        Application();
        ~Application() = default;

        entt::sink<void(float)> UpdateSlot;
        entt::sink<void(BatchRenderer&)> RenderSlot;
        entt::sink<void(BatchRenderer&)> LateRenderSlot;
        entt::sink<void(unsigned int, unsigned int)> WindowResizedSlot;
        entt::sink<void(bool)> WindowFocusChangedSlot;
        entt::sink<void()> WindowClosedSlot;

        static Application& Get();

        FileSystem& GetFileSystem();
        GameWindow& GetWindow();
        GameLoop& GetGameLoop();
        ResourceManager& GetResourceManager();
        AudioEngine& GetAudioEngine();
        Graphics& GetGraphics();
        InputManager& GetInputManager();
        DebugUI& GetDebugUI();

        //Returns wether the application is suspended or not
        bool IsSuspended();

    private:
        //Allow the main function to access the Run method
        friend int ::main(int argc, char** argv);

        //The Run() method is responsible for calling the Update() and Draw() methods
        //if the application isn't in a suspended state
        void Run();

        //GameLoopCallback methods
        void OnUpdate(float delta) override;
        void OnDraw() override;

        //GameWindowCallback methods
        void OnWindowResized(unsigned int, unsigned int) override;
        void OnWindowClosed() override;
        void OnWindowFocusChanged(bool isFocused) override;

        //Member variables
        std::function<void()> m_ShutdownCallback;

        entt::sigh<void(float)> m_UpdateSignal;
        entt::sigh<void(BatchRenderer&)> m_RenderSignal;
        entt::sigh<void(BatchRenderer&)> m_LateRenderSignal;
        entt::sigh<void(unsigned int, unsigned int)> m_WindowResizedSignal;
        entt::sigh<void(bool)> m_WindowFocusChangedSignal;
        entt::sigh<void()> m_WindowClosedSignal;

        enum class State
        {
            Splash = 0,
            FadeOut,
            FadeIn,
            Game
        };

        static Application* s_Instance;
        std::unique_ptr<GameWindow> m_GameWindow;
        std::unique_ptr<GameLoop> m_GameLoop;
        std::unique_ptr<ResourceManager> m_ResourceManager;
        std::unique_ptr<AudioEngine> m_AudioEngine;
        std::unique_ptr<Graphics> m_Graphics;
        std::unique_ptr<InputManager> m_InputManager;
        std::unique_ptr<DebugUI> m_DebugUI;
        std::unique_ptr<BatchRenderer> m_BatchRenderer;
        FileSystem m_FileSystem;
        HardwareInfo m_HardwareInfo;
        State m_State;
        std::unique_ptr<Sprite> m_SplashLogo;
        double m_SplashElapsed;
        double m_FadeElapsed;
        float m_FadeAlpha;
        bool m_IsSuspended;
        bool m_IsRunning;

        static const Color kSplashBackgroundColor;
        static const double kSplashDuration;
        static const double kFadeDuration;
    };
}

#pragma once

#include "TimeStep.h"
#include <entt.hpp>


namespace GameDev2D
{
    class GameLoopCallback
    {
    public:
        virtual ~GameLoopCallback() {}
        virtual void OnUpdate(float delta) = 0;
        virtual void OnDraw() = 0;
    };

    class GameLoop
    {
    public:
        GameLoop(GameLoopCallback* callback);
        ~GameLoop();

        void Step();

        float GetDelta();
        float GetElapsed();
        unsigned int GetFPS();

        static double Now();

    private:
        void OnWindowFocusChanged(bool isFocused);

        struct Stats
        {
            Stats() : fps(0), frameTime(0.0f) {}

            unsigned int fps;
            float frameTime;
        };

        GameLoopCallback* m_Callback;
        TimeStep m_TimeStep;
        Stats m_Stats;
        uint32_t m_Frames;
        uint32_t m_Updates;
        float m_UpdateTimer;
        float m_SecondTimer;
    };
}
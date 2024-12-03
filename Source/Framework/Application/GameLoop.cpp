#include "GameLoop.h"
#include "Application.h"
#include <GLFW/glfw3.h>


namespace GameDev2D
{
	GameLoop::GameLoop(GameLoopCallback* callback) :
		m_Callback(callback),
		m_TimeStep(0),
		m_Stats(),
		m_Frames(0),
		m_Updates(0),
		m_UpdateTimer(0),
		m_SecondTimer(0)
	{
		Application::Get().WindowFocusChangedSlot.connect<&GameLoop::OnWindowFocusChanged>(this);
	}

	GameLoop::~GameLoop()
	{
		Application::Get().WindowFocusChangedSlot.disconnect<&GameLoop::OnWindowFocusChanged>(this);
	}

	void GameLoop::Step()
	{
		double now = Now();
		m_TimeStep.Update(now);

		m_Callback->OnUpdate(static_cast<float>(m_TimeStep.GetDelta()));
		m_Updates++; 

		m_Callback->OnDraw();
		m_Frames++;

		if (glfwGetTime() - m_SecondTimer > 1.0f)
		{
			m_SecondTimer += 1.0f;
			m_Stats.fps = m_Frames;
			m_Stats.frameTime = 1000.0f / static_cast<float>(m_Frames);
			m_Updates = 0;
			m_Frames = 0;
		}
	}

	float GameLoop::GetDelta()
	{
		return static_cast<float>(m_TimeStep.GetDelta());
	}

	float GameLoop::GetElapsed()
	{
		return static_cast<float>(m_TimeStep.GetElapsed());
	}

	unsigned int GameLoop::GetFPS()
	{
		return m_Stats.fps;
	}

	double GameLoop::Now()
	{
		return glfwGetTime();
	}

	void GameLoop::OnWindowFocusChanged(bool isFocused)
	{
		if (isFocused)
		{
			m_TimeStep.Update(Now());
		}
	}
}

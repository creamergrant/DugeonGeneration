#include "Profile.h"
#include "Log.h"
#include "../Application/GameLoop.h"


namespace GameDev2D
{
    Profile::Profile(const std::string& label) :
        m_Label(label),
        m_StartTime(0),
        m_HasEnded(false)
    {
        Begin();
    }

    Profile::~Profile()
    {
        End();
    }

    void Profile::Begin()
    {
#if DEBUG
        m_StartTime = GameLoop::Now();
#endif
        m_HasEnded = false;
    }

    double Profile::End()
    {
        double duration = 0.0;

        if (!m_HasEnded)
        {
            m_HasEnded = true;

#if DEBUG
            double now = GameLoop::Now();
            duration = now - m_StartTime;

            Log::Message(LogVerbosity::Profiling, "[Profile]  %s: % f", m_Label.c_str(), duration);
#endif
        }

        return duration;
    }
}
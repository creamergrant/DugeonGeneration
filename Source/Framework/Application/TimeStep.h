#pragma once


namespace GameDev2D
{
    class TimeStep
    {
    public:
        TimeStep(double initialTime)
            : m_Timestep(0), m_LastTime(initialTime), m_Elapsed(0)
        {
        }

        void Update(double currentTime)
        {
            m_Timestep = currentTime - m_LastTime;
            m_LastTime = currentTime;
            m_Elapsed += m_Timestep;
        }

        double GetDelta() const { return m_Timestep; }
        double GetElapsed() const { return m_Elapsed; }
        double GetDeltaMS() const { return m_Timestep * 1000.0; }
        double GetElapsedMS() const { return m_Elapsed * 1000.0; }

    private:
        double m_Timestep;
        double m_LastTime;
        double m_Elapsed;
    };

}
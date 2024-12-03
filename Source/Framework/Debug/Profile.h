#pragma once

#include <string>


namespace GameDev2D
{
    class Profile
    {
    public:
        Profile(const std::string& label);
        ~Profile();

        //Call when you want to start profiling 
        void Begin();

        //Call when you are done profiling, it will return how long the operation took (in seconds)
        double End();

    private:
        std::string m_Label;
        double m_StartTime;
        bool m_HasEnded;
    };
}

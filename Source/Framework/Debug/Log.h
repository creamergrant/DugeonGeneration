#pragma once

#include <stdarg.h>

#define THROW_EXCEPTION_ON_ERROR 1
#define LOG_TO_FILE 0
#define LOG_FILE "/Log.txt"


namespace GameDev2D
{
    //Verbosity log levels
    enum class LogVerbosity
    {
        None = 0x0000,
        Debug = 0x0001,
        Application = 0x0002,
        Keyboard = 0x0004,
        Mouse = 0x00008,
        Gamepad = 0x0010,
        Input = Keyboard | Mouse | Gamepad,
        Graphics = 0x0020,
        Resources = 0x0040,
        Shaders = 0x0080,
        Audio = 0x0100,
        UI = 0x0200,
        Collision = 0x0400,
        Profiling = 0x0800,
        Memory = 0x1000,
        Hardware = 0x2000,
        All = Debug | Application | Input | Graphics | Resources | Shaders | Audio | UI | Collision | Profiling | Memory | Hardware
    };

    const unsigned int LOG_VERBOSITY_MASK = static_cast<unsigned int>(LogVerbosity::Debug) | static_cast<unsigned int>(LogVerbosity::Application) | static_cast<unsigned int>(LogVerbosity::Hardware) | static_cast<unsigned int>(LogVerbosity::Profiling);

    struct Log
    {
        //Calling Init() will ensure that the log file is properly created on the hard drive
        static void Init();

        //Used to Log a message with a variable amount of arguments, the
        //verbosity level for these logs is debug (VerbosityLevel_Debug).
        //If the LOG_TO_FILE to file preproc is enabled then this method
        //will log to a file in addition to the output window.
        static void Message(std::string message, ...);

        //Same as the Message() function above, except you can specify the
        //verbosity of the log messages.
        static void Message(LogVerbosity verbosity, std::string message, ...);

        //Used to specically log errors, it has no verbosity level, and can
        //throw an exception.
        static void Error(bool throwException, LogVerbosity verbosity, std::string message, ...);

    private:
        //Conveniance method used to log messages
        static void Output(bool error, LogVerbosity verbosity, const char* aOutput, va_list aArgumentsList);
    
        //Member variables
        static bool s_IsInitialized;
    };
}

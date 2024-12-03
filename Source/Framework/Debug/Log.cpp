#include "Log.h"
#include <GameDev2D.h>
#include <stdio.h>
#include <string>
#include <stdexcept>


namespace GameDev2D
{
    bool Log::s_IsInitialized = false;

    void Log::Init()
    {
#if LOG_TO_FILE
        if (s_IsInitialized == false)
        {
            //Get the Path for the Working Directory and append the Log filename to it
            std::string path = Application::Get().GetFileSystem().GetApplicationDirectory();
            path.append(LOG_FILE);

            //Open the log file, wiping any existing data in the file
            FILE* logFile;
            logFile = fopen(path.c_str(), "w");

            //Make sure the file opened, if it did close the file
            if (logFile != NULL)
            {
                s_IsInitialized = true;
                fprintf(logFile, "GameDev2D initiated\n\n");
                fclose(logFile);
            }
        }
#endif
    }

    void Log::Message(std::string aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, LogVerbosity::Debug, aMessage.c_str(), arguments);
        va_end(arguments);
#endif
    }


    void Log::Message(LogVerbosity aVerbosity, std::string aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(false, aVerbosity, aMessage.c_str(), arguments);
        va_end(arguments);
#endif
    }

    void Log::Error(bool aThrowException, LogVerbosity aVerbosity, std::string aMessage, ...)
    {
#if DEBUG || _DEBUG
        va_list arguments;
        va_start(arguments, aMessage);
        Log::Output(true, aVerbosity, aMessage.c_str(), arguments);
        va_end(arguments);


        
#if THROW_EXCEPTION_ON_ERROR
        //Should an exception be thrown?
        if (aThrowException == true)
        {
            char outputBuffer[8192];
            vsnprintf(outputBuffer, 8192, aMessage.c_str(), arguments);

            throw std::runtime_error(outputBuffer);
        }

        //End the variable argument list
        va_end(arguments);
#endif
#endif
    }

    void Log::Output(bool aError, LogVerbosity aVerbosity, const char* aOutput, va_list aArgumentsList)
    {
#if DEBUG || _DEBUG
        //Is this an error?
        if (aError == true)
        {
            ////Output to the window's 'Output' window
            OutputDebugStringA("[ERROR]");
            char outputBuffer[1024];
            vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
            OutputDebugStringA(outputBuffer);
            OutputDebugStringA("\n");

#if LOG_TO_FILE
            if (s_IsInitialized == true)
            {
                //Get the path for the log file
                std::string path = Application::Get().GetFileSystem().GetApplicationDirectory();
                path.append(LOG_FILE);

                //Open the file at the path, if the assert is hit it means the log file couldn't be opened
                FILE* logFile;
                logFile = fopen(path.c_str(), "a+");
                assert(logFile != nullptr);

                //Write the log to the file
                if (logFile != NULL)
                {
                    fprintf(logFile, "[ERROR]");
                    vfprintf(logFile, aOutput, aArgumentsList);
                    fprintf(logFile, "\n");
                    fclose(logFile);
                }
            }
#endif
        }
        else
        {
            if ((static_cast<unsigned int>(aVerbosity) & LOG_VERBOSITY_MASK) != 0)
            {
                //Output to the window's 'Output' window
                char outputBuffer[1024];
                vsnprintf(outputBuffer, 1024, aOutput, aArgumentsList);
                OutputDebugStringA(outputBuffer);
                OutputDebugStringA("\n");

#if LOG_TO_FILE
                if (s_IsInitialized == true)
                {
                    //Get the path for the log file
                    std::string path = Application::Get().GetFileSystem().GetApplicationDirectory();
                    path.append(LOG_FILE);

                    //Open the file at the path, if the assert is hit it means the log file couldn't be opened
                    FILE* logFile;
                    logFile = fopen(path.c_str(), "a+");
                    assert(logFile != nullptr);

                    //Write the log to the file
                    if (logFile != NULL)
                    {
                        vfprintf(logFile, aOutput, aArgumentsList);
                        fprintf(logFile, "\n");
                        fclose(logFile);
                    }
                }
#endif
            }
        }
#endif
    }
}
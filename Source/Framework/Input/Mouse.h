#pragma once

#include "../Math/Vector2.h"
#include <entt.hpp>
#include <map>
#include <string>


namespace GameDev2D
{
    enum class MouseButton
    {
        None = -1,
        Left = 0,
        Right,
        Middle
    };

    enum class MouseButtonState
    {
        None = -1,
        Pressed = 0,
        Released
    };

    class Mouse
    {
    public:
        Mouse();
        ~Mouse();
        
        entt::sink<void(MouseButton, MouseButtonState, float, float)> MouseButtonEventSlot;
        entt::sink<void(float, float, float, float)> MouseMovedEventSlot;
        entt::sink<void(float, float)> MouseScrolledEventSlot;

        //Returns the current mouse's position
        Vector2 GetPosition();

        //Returns the current mouse's previous position
        Vector2 GetPreviousPosition();
        
        //Returns wether a mouse button is pressed or not
        bool IsButtonPressed(MouseButton button);

        //Converts a MouseButton to a string
        static std::string ButtonToString(MouseButton button);

        //Converts a MouseButtonState to a string
        static std::string ButtonStateToString(MouseButtonState state);
    private:
        friend class GameWindow;

        void OnUpdate(float delta);
        void OnWindowFocusChange(bool isFocused);

        //Handle mouse events, called from GameWindow
        void HandleButtonEvent(MouseButton button, MouseButtonState state);
        void HandleMouseMoved(float x, float y);
        void HandleScroll(float deltaX, float deltaY);

        //Manages the internal State for each Mouse Button
        struct State
        {
            State() : duration(0.0), buttonState(MouseButtonState::Released)
            {}

            float duration;
            MouseButtonState buttonState;
        };

        //Member variables
        Vector2 m_Position;
        Vector2 m_PreviousPosition;
        std::unordered_map<MouseButton, State> m_ButtonStates;
        entt::sigh<void(MouseButton, MouseButtonState, float, float)> m_MouseButtonEventSignal;
        entt::sigh<void(float, float, float, float)> m_MouseMovedEventSignal;
        entt::sigh<void(float, float)> m_MouseScrolledEventSignal;
    };
}


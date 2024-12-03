#include "Mouse.h"
#include "../Application/Application.h"


namespace GameDev2D
{
    Mouse::Mouse() : 
        MouseButtonEventSlot(m_MouseButtonEventSignal),
        MouseMovedEventSlot(m_MouseMovedEventSignal),
        MouseScrolledEventSlot(m_MouseScrolledEventSignal),
        m_Position(0.0f, 0.0f),
        m_PreviousPosition(0.0f, 0.0f)
    {
        Application::Get().UpdateSlot.connect<&Mouse::OnUpdate>(this);
        Application::Get().WindowFocusChangedSlot.connect<&Mouse::OnWindowFocusChange>(this);
    }

    Mouse::~Mouse()
    {
        Application::Get().UpdateSlot.disconnect<&Mouse::OnUpdate>(this);
        Application::Get().WindowFocusChangedSlot.disconnect<&Mouse::OnWindowFocusChange>(this);
    }
    
    Vector2 Mouse::GetPosition()
    {
        return m_Position;
    }
    
    Vector2 Mouse::GetPreviousPosition()
    {
        return m_PreviousPosition;
    }

    bool Mouse::IsButtonPressed(MouseButton aButton)
    {
        return m_ButtonStates[aButton].buttonState == MouseButtonState::Pressed;
    }

    std::string Mouse::ButtonToString(MouseButton button)
    {
        switch (button)
        {
            case MouseButton::Left:
                return "Left";
            case MouseButton::Middle:
                return "Middle";  
            case MouseButton::Right:
                return "Right";
        }
        return "Unknown";
    }

    std::string Mouse::ButtonStateToString(MouseButtonState state)
    {
        switch (state)
        {
        case MouseButtonState::Pressed:
            return "Pressed";
        case MouseButtonState::Released:
            return "Released";
        }
        return "Unknown";
    }

    void Mouse::OnUpdate(float delta)
    {
        for (std::unordered_map<MouseButton, State>::iterator i = m_ButtonStates.begin(); i != m_ButtonStates.end(); ++i)
        {
            if (i->second.buttonState == MouseButtonState::Pressed)
            {
                i->second.duration += delta;
            }
        }
    }

    void Mouse::OnWindowFocusChange(bool isFocused)
    {
        if (!isFocused)
        {
            for (std::unordered_map<MouseButton, State>::iterator it = m_ButtonStates.begin(); it != m_ButtonStates.end(); ++it)
            {
                if (it->second.buttonState == MouseButtonState::Pressed)
                {
                    it->second.buttonState = MouseButtonState::Released;
                    it->second.duration = 0.0;
                }
            }
        }
    }

    void Mouse::HandleButtonEvent(MouseButton button, MouseButtonState state)
    {
        //Set the button state
        m_ButtonStates[button].buttonState = state;

        //Log the key event
        Log::Message(LogVerbosity::Mouse, "[Mouse] button: (%s) %s", ButtonToString(button).c_str(), ButtonStateToString(state).c_str());
        
        //Reset the duration on button released
        if (state == MouseButtonState::Released)
        {
            m_ButtonStates[button].duration = 0.0;
        }

        //Publish the mouse button signal
        m_MouseButtonEventSignal.publish(button, state, GetPosition().x, GetPosition().y);
    }

    void Mouse::HandleMouseMoved(float x, float y)
    {
        m_PreviousPosition = m_Position;
        m_Position = Vector2(x, y);

        //Log the key event
        Log::Message(LogVerbosity::Mouse, "[Mouse] moved: (%f, %f)", x, y);

        //Publish the mouse moved signal
        m_MouseMovedEventSignal.publish(m_Position.x, m_Position.y, m_PreviousPosition.x, m_PreviousPosition.y);
    }

    void Mouse::HandleScroll(float deltaX, float deltaY)
    {
        //Log the key event
        Log::Message(LogVerbosity::Mouse, "[Mouse] scrolled: (%f, %f)", deltaX, deltaY);

        //Publish the mouse scrolled signal
        m_MouseScrolledEventSignal.publish(deltaX, deltaY);
    }
}
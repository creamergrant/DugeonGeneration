#include "InputManager.h"


namespace GameDev2D
{
    InputManager::InputManager() :
        m_Keyboard(),
        m_Mouse(),
        m_Gamepads{GamepadPort::One, GamepadPort::Two, GamepadPort::Three, GamepadPort::Four }
    {
    }

    Keyboard& InputManager::GetKeyboard()
    {
        return m_Keyboard;
    }

    Mouse& InputManager::GetMouse()
    {
        return m_Mouse;
    }

    Gamepad& InputManager::GetGamepad(GamepadPort port)
    {
        return m_Gamepads[static_cast<int>(port)];
    }
}
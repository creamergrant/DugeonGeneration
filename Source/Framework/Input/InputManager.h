#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"


namespace GameDev2D
{
    // The InputManager game service handles all input for all platforms, including mouse, keyboard and GamePad
    class InputManager
    {
    public:
        InputManager();
        ~InputManager() = default;

        //Returns the Keyboard object, which manages the button state for all the keys
        Keyboard& GetKeyboard();

        //Returns the Mouse object, which manages the position and button states for the mouse buttons
        Mouse& GetMouse();
    
        //
        Gamepad& GetGamepad(GamepadPort port = GamepadPort::One);

    private:
        //Keyboard data
        Keyboard m_Keyboard;
        Mouse m_Mouse;
        Gamepad m_Gamepads[NUM_GAMEPAD_PORTS];
    };
}

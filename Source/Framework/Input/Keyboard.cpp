#include "Keyboard.h"
#include "../Application/Application.h"


namespace GameDev2D
{
    Keyboard::Keyboard() :
        KeyEventSlot(m_KeyEventSignal)
    {
        Application::Get().UpdateSlot.connect<&Keyboard::OnUpdate>(this);
        Application::Get().WindowFocusChangedSlot.connect<&Keyboard::OnWindowFocusChange>(this);
    }
    
    Keyboard::~Keyboard()
    {
        Application::Get().UpdateSlot.disconnect<&Keyboard::OnUpdate>(this);
        Application::Get().WindowFocusChangedSlot.disconnect<&Keyboard::OnWindowFocusChange>(this);
    }

    Keyboard::State Keyboard::GetState(KeyCode keyCode)
    {
        return m_KeyStates[keyCode];
    }

    std::string Keyboard::KeyStateToString(KeyState aKeyState)
    {
        switch (aKeyState)
        {
        case KeyState::Down:
            return "Key Down";
        case KeyState::Up:
            return "Key Up";
        case KeyState::Repeat:
            return "Key Repeat";
        }

        return "Unknown";
    }

    std::string Keyboard::KeyCodeToString(KeyCode aKeyCode)
    {
        switch (aKeyCode)
        {
        case KeyCode::Up:
            return "Up";
        case KeyCode::Down:
            return "Down";
        case KeyCode::Right:
            return "Right";
        case KeyCode::Left:
            return "Left";

        case KeyCode::Space:
            return "Space";
        case KeyCode::Enter:
            return "Enter";
        case KeyCode::Tab:
            return "Tab";
        case KeyCode::Escape:
            return "Escape";
        case KeyCode::Backspace:
            return "Backspace";
        case KeyCode::Delete:
            return "Delete";

        case KeyCode::A:
            return "A";
        case KeyCode::B:
            return "B";
        case KeyCode::C:
            return "C";
        case KeyCode::D:
            return "D";
        case KeyCode::E:
            return "E";
        case KeyCode::F:
            return "F";
        case KeyCode::G:
            return "G";
        case KeyCode::H:
            return "H";
        case KeyCode::I:
            return "I";
        case KeyCode::J:
            return "J";
        case KeyCode::K:
            return "K";
        case KeyCode::L:
            return "L";
        case KeyCode::M:
            return "M";
        case KeyCode::N:
            return "N";
        case KeyCode::O:
            return "O";
        case KeyCode::P:
            return "P";
        case KeyCode::Q:
            return "Q";
        case KeyCode::R:
            return "R";
        case KeyCode::S:
            return "S";
        case KeyCode::T:
            return "T";
        case KeyCode::U:
            return "U";
        case KeyCode::V:
            return "V";
        case KeyCode::W:
            return "W";
        case KeyCode::X:
            return "X";
        case KeyCode::Y:
            return "Y";
        case KeyCode::Z:
            return "Z";

        case KeyCode::Zero:
            return "0";
        case KeyCode::One:
            return "1";
        case KeyCode::Two:
            return "2";
        case KeyCode::Three:
            return "3";
        case KeyCode::Four:
            return "4";
        case KeyCode::Five:
            return "5";
        case KeyCode::Six:
            return "6";
        case KeyCode::Seven:
            return "7";
        case KeyCode::Eight:
            return "8";
        case KeyCode::Nine:
            return "9";

        case KeyCode::Minus:
            return "-";
        case KeyCode::Equal:
            return "=";

        case KeyCode::Semicolon:
            return ";";
        case KeyCode::Apostrophe:
            return "'";

        case KeyCode::Comma:
            return ",";
        case KeyCode::Period:
            return ".";

        case KeyCode::Slash:
            return "/";
        case KeyCode::Backslash:
            return "\\";

        case KeyCode::RightBracket:
            return "]";
        case KeyCode::LeftBracket:
            return "[";

        case KeyCode::RightShift:
            return "Right Shift";
        case KeyCode::LeftShift:
            return "Left Shift";
        case KeyCode::RightControl:
            return "Right Control";
        case KeyCode::LeftControl:
            return "Left Control";
        case KeyCode::RightAlt:
            return "Right Alt";
        case KeyCode::LeftAlt:
            return "Left Alt";

        case KeyCode::NumLock:
            return "Num Lock";
        case KeyCode::CapsLock:
            return "Caps Lock";
        case KeyCode::ScrollLock:
            return "Scroll Lock";

        case KeyCode::Pause:
            return "Pause";
        case KeyCode::Insert:
            return "Insert";
        case KeyCode::Home:
            return "Home";
        case KeyCode::End:
            return "End";
        case KeyCode::PageUp:
            return "Page Up";
        case KeyCode::PageDown:
            return "Page Down";

        case KeyCode::F1:
            return "F1";
        case KeyCode::F2:
            return "F2";
        case KeyCode::F3:
            return "F3";
        case KeyCode::F4:
            return "F4";
        case KeyCode::F5:
            return "F5";
        case KeyCode::F6:
            return "F6";
        case KeyCode::F7:
            return "F7";
        case KeyCode::F8:
            return "F8";
        case KeyCode::F9:
            return "F9";
        case KeyCode::F10:
            return "F10";
        case KeyCode::F11:
            return "F11";
        case KeyCode::F12:
            return "F12";
        case KeyCode::F13:
            return "F13";
        case KeyCode::F14:
            return "F14";
        case KeyCode::F15:
            return "F15";

        default:
            break;
        }

        return "Unknown";
    }

    void Keyboard::OnUpdate(float aDelta)
    {
        //If a key is down, then update the duration in which it's been held down for
        for (std::unordered_map<KeyCode, State>::iterator i = m_KeyStates.begin(); i != m_KeyStates.end(); ++i)
        {
            if (i->second.keyState == KeyState::Down || i->second.keyState == KeyState::Repeat)
            {
                i->second.duration += aDelta;
            }
        }
    }

    void Keyboard::OnWindowFocusChange(bool isFocused)
    {
        if (!isFocused)
        {
            for (std::unordered_map<KeyCode, State>::iterator it = m_KeyStates.begin(); it != m_KeyStates.end(); ++it)
            {
                if (it->second.keyState == KeyState::Down)
                {
                    it->second.keyState = KeyState::Up;
                    it->second.duration = 0.0;
                }
            }
        }
    }

    void Keyboard::HandleKeyEvent(KeyCode keyCode, KeyState keyState)
    {
        //Set the key state
        m_KeyStates[keyCode].keyState = keyState;

        //Log the key event
        Log::Message(LogVerbosity::Keyboard, "[Keyboard] (%s) %s", KeyCodeToString(keyCode).c_str(), KeyStateToString(keyState).c_str());

        //Reset the duration on key up
        if (keyState == KeyState::Up)
        {
            m_KeyStates[keyCode].duration = 0.0; 
        }

        //Publish the key event signal
        m_KeyEventSignal.publish(keyCode, keyState);
    }
}

#include "Gamepad.h"
#include "../Application/Application.h"
#include <GLFW/glfw3.h>


namespace GameDev2D
{
	Gamepad::Gamepad(GamepadPort port) :
		IsConnectedSlot(m_IsConnectedSignal),
		IsDisconnectedSlot(m_IsDisconnectedSignal),
		LeftThumbstickSlot(m_LeftThumbstickSignal),
		RightThumbstickSlot(m_RightThumbstickSignal),
		LeftTriggerSlot(m_LeftTriggerSignal),
		RightTriggerSlot(m_RightTriggerSignal),
		ButtonPressedSlot(m_ButtonPressedSignal),
		ButtonReleasedSlot(m_ButtonReleasedSignal),
		m_Port(port),
		m_LeftThumbStick(Vector2::Zero),
		m_RightThumbStick(Vector2::Zero),
		m_LeftTrigger(0.0f),
		m_RightTrigger(0.0f),
		m_IsConnected(false)
	{
		Application::Get().UpdateSlot.connect<&Gamepad::OnUpdate>(this);
	}

	Gamepad::~Gamepad()
	{
		Application::Get().UpdateSlot.disconnect<&Gamepad::OnUpdate>(this);
	}

	bool Gamepad::IsConnected()
	{
		return m_IsConnected;
	}

	bool Gamepad::IsButtonPressed(GamepadButton button)
	{
		return m_Buttons[button].isPressed;
	}

	Vector2 Gamepad::GetLeftThumbStick()
	{
		return m_LeftThumbStick;
	}

	Vector2 Gamepad::GetRightThumbStick()
	{
		return m_RightThumbStick;
	}

	float Gamepad::GetLeftTrigger()
	{
		return m_LeftTrigger;
	}

	float Gamepad::GetRightTrigger()
	{
		return m_RightTrigger;
	}

	std::string Gamepad::ButtonToString(GamepadButton button)
	{
		switch (button)
		{
		case GamepadButton::A:
			return "A";
		case GamepadButton::B:
			return "B";
		case GamepadButton::X:
			return "X";
		case GamepadButton::Y:
			return "Y";
		case GamepadButton::L1:
			return "L1";
		case GamepadButton::R1:
			return "R1";
		case GamepadButton::Back:
			return "Back";
		case GamepadButton::Start:
			return "Start";
		case GamepadButton::Guide:
			return "Guide";
		case GamepadButton::L3:
			return "L3";
		case GamepadButton::R3:
			return "R3";
		case GamepadButton::DpadUp:
			return "Dpad Up";
		case GamepadButton::DpadRight:
			return "Dpad Right";
		case GamepadButton::DpadDown:
			return "Dpad Down";
		case GamepadButton::DpadLeft:
			return "Dpad Left";
		}
		return "Unknown";
	}

	void Gamepad::OnUpdate(float delta)
	{
		int port = static_cast<int>(m_Port);

		if (!m_IsConnected)
		{
			int present = glfwJoystickPresent(port);
			if (present == GLFW_TRUE && glfwJoystickIsGamepad(port))
			{
				m_IsConnected = true;
			}
		}

		if (m_IsConnected)
		{
			GLFWgamepadstate state;
			if (glfwGetGamepadState(port, &state) != GLFW_FALSE)
			{
				RefreshState(delta, state);
			}
		}
	}

	void Gamepad::OnConnected()
	{
		m_IsConnected = true;
		m_IsConnectedSignal.publish(m_Port);
	}

	void Gamepad::OnDisconnected()
	{
		m_IsConnected = false;

		m_LeftTrigger = 0.0f;
		m_RightTrigger = 0.0f;
		m_LeftThumbStick = Vector2::Zero;
		m_RightThumbStick = Vector2::Zero;

		m_IsDisconnectedSignal.publish(m_Port);
	}

	void Gamepad::RefreshState(double delta, const GLFWgamepadstate& state)
	{
		//Handle the left thumb stick
		Vector2 previousLeftThumbStick = m_LeftThumbStick;
		m_LeftThumbStick.x = state.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
		m_LeftThumbStick.y = state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y];

		//Is the magnitude less than the deadzone? if it is, zero the analog input
		if (m_LeftThumbStick.Length() < GAMEPAD_DEADZONE)
		{
			m_LeftThumbStick = Vector2::Zero;
		}

		if (previousLeftThumbStick != m_LeftThumbStick)
		{
			Log::Message(LogVerbosity::Gamepad, "[Gamepad] left thumbstick: (%f,%f)", m_LeftThumbStick.x, m_LeftThumbStick.y);
			m_LeftThumbstickSignal.publish(m_Port, m_LeftThumbStick);
		}

		//Handle the right thumb stick
		Vector2 previousRightThumbStick = m_RightThumbStick;
		m_RightThumbStick.x = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
		m_RightThumbStick.y = state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];

		//Is the magnitude less than the deadzone? if it is, zero the analog input
		if (m_RightThumbStick.Length() < GAMEPAD_DEADZONE)
		{
			m_RightThumbStick = Vector2::Zero;
		}

		if (previousRightThumbStick != m_RightThumbStick)
		{
			Log::Message(LogVerbosity::Gamepad, "[Gamepad] right thumbstick: (%f,%f)", m_RightThumbStick.x, m_RightThumbStick.y);
			m_RightThumbstickSignal.publish(m_Port, m_RightThumbStick);
		}

		//Handle the left trigger
		float previousLeftTrigger = m_LeftTrigger;
		m_LeftTrigger = (state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.0f) / 2.0f;

		if (m_LeftTrigger != previousLeftTrigger)
		{
			Log::Message(LogVerbosity::Gamepad, "[Gamepad] left trigger: %f", m_LeftTrigger);
			m_LeftTriggerSignal.publish(m_Port, m_LeftTrigger);
		}

		//Handle the right trigger
		float previousRightTrigger = m_RightTrigger;
		m_RightTrigger = (state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.0f) / 2.0f;

		if (m_RightTrigger != previousRightTrigger)
		{
			Log::Message(LogVerbosity::Gamepad, "[Gamepad] right trigger: %f", m_RightTrigger);
			m_RightTriggerSignal.publish(m_Port, m_RightTrigger);
		}
		
		//Handle button input
		RefreshButton(delta, GamepadButton::A, state.buttons[GLFW_GAMEPAD_BUTTON_A] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::B, state.buttons[GLFW_GAMEPAD_BUTTON_B] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::X, state.buttons[GLFW_GAMEPAD_BUTTON_X] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::Y, state.buttons[GLFW_GAMEPAD_BUTTON_Y] == GLFW_PRESS);

		RefreshButton(delta, GamepadButton::L1, state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::R1, state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] == GLFW_PRESS);
		
		RefreshButton(delta, GamepadButton::Back, state.buttons[GLFW_GAMEPAD_BUTTON_BACK] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::Start, state.buttons[GLFW_GAMEPAD_BUTTON_START] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::Guide, state.buttons[GLFW_GAMEPAD_BUTTON_GUIDE] == GLFW_PRESS);

		RefreshButton(delta, GamepadButton::L3, state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::R3, state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB] == GLFW_PRESS);

		RefreshButton(delta, GamepadButton::DpadUp, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::DpadRight, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::DpadDown, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN] == GLFW_PRESS);
		RefreshButton(delta, GamepadButton::DpadLeft, state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT] == GLFW_PRESS);
	}

	void Gamepad::RefreshButton(double delta, GamepadButton button, bool isPressed)
	{
		if (m_Buttons[button].isPressed != isPressed)
		{
			m_Buttons[button].isPressed = isPressed;

			if (isPressed)
			{
				Log::Message(LogVerbosity::Gamepad, "[Gamepad] button: %s pressed", ButtonToString(button).c_str());
				m_ButtonPressedSignal.publish(m_Port, button);
			}
			else
			{
				Log::Message(LogVerbosity::Gamepad, "[Gamepad] button: %s released", ButtonToString(button).c_str());
				m_ButtonReleasedSignal.publish(m_Port, button);
				m_Buttons[button].duration = 0.0;
			}
		}
		else
		{
			if (m_Buttons[button].isPressed)
			{
				m_Buttons[button].duration += delta;
			}
		}
	}
}

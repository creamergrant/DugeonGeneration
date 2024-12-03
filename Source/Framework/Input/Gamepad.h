#pragma once

#include "../Math/Vector2.h"
#include <entt.hpp>
#include <unordered_map>


struct GLFWgamepadstate;

namespace GameDev2D
{
	const int NUM_GAMEPAD_PORTS = 4;
	const float GAMEPAD_DEADZONE = 0.2f;

	enum class GamepadPort
	{
		One = 0,
		Two,
		Three,
		Four
	};

	enum class GamepadButton
	{
		// From glfw3.h

		A = 0,   //Cross on DS4
		B = 1,   //Circle on DS4
		X = 2,   //Square on DS4
		Y = 3,   //Triangle on DS4

		L1 = 4, 
		R1 = 5,

		Back = 6, 
		Start = 7,
		Guide = 8,

		L3 = 9,  //Left thumbstick
		R3 = 10, //Right thumbstick

		DpadUp = 11,
		DpadRight = 12,
		DpadDown = 13,
		DpadLeft = 14
	};


	class Gamepad
	{
	public:
		Gamepad(GamepadPort port);
		~Gamepad();

		entt::sink<void(GamepadPort)> IsConnectedSlot;
		entt::sink<void(GamepadPort)> IsDisconnectedSlot;
		entt::sink<void(GamepadPort, Vector2)> LeftThumbstickSlot;
		entt::sink<void(GamepadPort, Vector2)> RightThumbstickSlot;
		entt::sink<void(GamepadPort, float)> LeftTriggerSlot;
		entt::sink<void(GamepadPort, float)> RightTriggerSlot;
		entt::sink<void(GamepadPort, GamepadButton)> ButtonPressedSlot;
		entt::sink<void(GamepadPort, GamepadButton)> ButtonReleasedSlot;

		//
		bool IsConnected();

		//Returns the Button is pressed or not
		bool IsButtonPressed(GamepadButton button);

		//Returns the value of the left thumb stick, values have a range from 0.0f to 1.0f
		Vector2 GetLeftThumbStick();

		//Returns the value of the right thumb stick, values have a range from 0.0f to 1.0f
		Vector2 GetRightThumbStick();

		//Returns the value of the left trigger, values have a range from 0.0f to 1.0f
		float GetLeftTrigger();

		//Returns the value of the right trigger, values have a range from 0.0f to 1.0f
		float GetRightTrigger();
		
		//Converts a GamepadButton to a string
		static std::string ButtonToString(GamepadButton button);

	private:
		friend class GameWindow;

		void OnUpdate(float delta);
		void OnConnected();
		void OnDisconnected();

		void RefreshState(double delta, const GLFWgamepadstate& state);
		void RefreshButton(double delta, GamepadButton button, bool isPressed);

		struct State
		{
			State() :
				duration(0.0),
				isPressed(false)
			{
			}

			State(const State& state) :
				duration(state.duration),
				isPressed(state.isPressed)
			{
			}

			double duration;
			bool isPressed;
		};
		
		entt::sigh<void(GamepadPort)> m_IsConnectedSignal;
		entt::sigh<void(GamepadPort)> m_IsDisconnectedSignal;
		entt::sigh<void(GamepadPort, Vector2)> m_LeftThumbstickSignal;
		entt::sigh<void(GamepadPort, Vector2)> m_RightThumbstickSignal;
		entt::sigh<void(GamepadPort, float)> m_LeftTriggerSignal;
		entt::sigh<void(GamepadPort, float)> m_RightTriggerSignal;
		entt::sigh<void(GamepadPort, GamepadButton)> m_ButtonPressedSignal;
		entt::sigh<void(GamepadPort, GamepadButton)> m_ButtonReleasedSignal;
		std::unordered_map<GamepadButton, State> m_Buttons;
		GamepadPort m_Port;
		Vector2 m_LeftThumbStick;
		Vector2 m_RightThumbStick;
		float m_LeftTrigger;
		float m_RightTrigger;
		bool m_IsConnected;
	};
}
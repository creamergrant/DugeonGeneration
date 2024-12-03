#pragma once

#include <string>


namespace GameDev2D
{
	struct Config
	{
		Config();

		void Load();

		struct Application
		{
			Application();

			bool disableSplash;
		};

		struct Window
		{
			Window();

			std::string title;
			unsigned int width;
			unsigned int height;
			bool fullscreen;
			bool vsync;
		};

		struct Renderer
		{
			Renderer();

			unsigned int clearColor;
		};

		struct Resources
		{
			Resources();

			bool autoLoad;
		};

		struct Debug
		{
			Debug();

			bool drawFps;
			bool drawElapsed;
			bool drawDelta;
			bool drawTotalDrawCalls;
			bool drawFrameDrawCalls;
			bool drawCameraPosition;
			bool drawSpriteOutline;
		};

		Application application;
		Window window;
		Renderer renderer;
		Resources resources;
		Debug debug;
	};
}
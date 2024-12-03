#include "Config.h"
#include "../Application/Application.h"
#include <json.h>
#include <fstream>


namespace GameDev2D
{
	Config::Config() :
		application(),
		window(),
		renderer(),
		resources(),
		debug()
	{}

	void Config::Load()
	{
		std::string path = GameDev2D::Application::Get().GetFileSystem().GetPlatformConfigPath();

		if (GameDev2D::Application::Get().GetFileSystem().DoesFileExistAtPath(path))
		{
			std::ifstream inputStream;
			inputStream.open(path.c_str(), std::ifstream::in);

			Json::Value root;
			Json::Reader reader;
			if (reader.parse(inputStream, root, false) == true)
			{
				const Json::Value applicationValue = root["application"];
				application.disableSplash = applicationValue["disableSplash"].asBool();

				const Json::Value windowValue = root["window"];
				window.title = windowValue["title"].asString();
				window.width = windowValue["width"].asUInt();
				window.height = windowValue["height"].asUInt();
				window.fullscreen = windowValue["fullscreen"].asBool();
				window.vsync = windowValue["vsync"].asBool();

				const Json::Value rendererValue = root["renderer"];
				std::string hexString = rendererValue["clearColor"].asString();
				renderer.clearColor = strtoul(hexString.c_str(), nullptr, 16);

				const Json::Value resourcesValue = root["resources"];
				resources.autoLoad = resourcesValue["autoLoad"].asBool();

				const Json::Value debugValue = root["debug"];
				debug.drawFps = debugValue["drawFps"].asBool();
				debug.drawElapsed = debugValue["drawElapsed"].asBool();
				debug.drawDelta = debugValue["drawDelta"].asBool();
				debug.drawTotalDrawCalls = debugValue["drawTotalDrawCalls"].asBool();
				debug.drawFrameDrawCalls = debugValue["drawFrameDrawCalls"].asBool();
				debug.drawCameraPosition = debugValue["drawCameraPosition"].asBool();	
				debug.drawSpriteOutline = debugValue["drawSpriteOutline"].asBool();
			}

			inputStream.close();
		}
	}

	Config::Application::Application() :
		disableSplash(false)
	{}

	Config::Window::Window() :
		title("GameDev2D"),
		width(480),
		height(360),
		fullscreen(false),
		vsync(false)
	{}

	Config::Renderer::Renderer() :
		clearColor(0x000000ff)
	{}

	Config::Resources::Resources() :
		autoLoad(false)
	{}

	Config::Debug::Debug() :
		drawFps(false),
		drawElapsed(false),
		drawDelta(false),
		drawTotalDrawCalls(false),
		drawFrameDrawCalls(false),
		drawCameraPosition(false),
		drawSpriteOutline(false)
	{}
}
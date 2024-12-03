#pragma once

#include <string>


namespace GameDev2D
{
	struct Resource;

	class FileSystem
	{
	public:
		FileSystem() = default;
		~FileSystem() = default;

		//Returns a path to the directory that the application is in
		std::string GetApplicationDirectory();

		//Returns a path for an asset in a specific sub-directory
		std::string GetPathForResource(const std::string& filename, const std::string& extension, const std::string& directory);
		std::string GetPathForResource(const Resource& resource);

		//Returns a path to the windows icon
		std::string GetPathForWindowsIcon();

		//Returns a path to the platform config file
		std::string GetPlatformConfigPath();

		//Returns wether a file exists at the supplied path
		bool DoesFileExistAtPath(const std::string& path);
	};
}
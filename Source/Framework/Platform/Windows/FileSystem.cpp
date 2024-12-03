#include "FileSystem.h"
#include "../Platform.h"
#include "../../Resources/Resource.h"

#if GAMEDEV2D_OS_WINDOWS
#include <Windows.h>
#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")
#endif


namespace GameDev2D
{
	std::string FileSystem::GetApplicationDirectory()
	{
		return ".";
	}

	std::string FileSystem::GetPathForResource(const std::string& filename, const std::string& extension, const std::string& directory)
	{
		return GetPathForResource(Resource(filename, extension, directory));
	}

	std::string FileSystem::GetPathForResource(const Resource& resource)
	{
		return GetApplicationDirectory() + "\\Assets\\" + resource.directory + "\\" + resource.filename + "." + resource.extension;
	}

	std::string FileSystem::GetPathForWindowsIcon()
	{
		return GetApplicationDirectory() + "\\Assets\\Icon\\Icon.png";
	}

	std::string FileSystem::GetPlatformConfigPath()
	{
		return GetApplicationDirectory() + "\\Config\\Info.json";
	}

	bool FileSystem::DoesFileExistAtPath(const std::string& path)
	{
		bool doesExist = false;
#if GAMEDEV2D_OS_WINDOWS
		DWORD fileAttributes = GetFileAttributesA(path.c_str());
		doesExist = (fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
#endif
		return doesExist;
	}
}
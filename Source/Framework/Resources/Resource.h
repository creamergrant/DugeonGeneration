#pragma once

#include <string>

namespace GameDev2D
{
	struct Resource
	{
		Resource(const std::string& filename, const std::string& extension, const std::string& directory) :
			filename(filename),
			extension(extension),
			directory(directory)
		{}

		bool IsValid() const { return filename.length() > 0 && extension.length() > 0; }

		const std::string filename;
		const std::string extension;
		const std::string directory;
	};
}
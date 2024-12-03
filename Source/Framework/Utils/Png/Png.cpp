#include "Png.h"
#include "../../Graphics/GraphicTypes.h"
#include <stb_image.h>


namespace GameDev2D
{
    bool Png::LoadFromPath(const std::string& path, ImageData& imageData, bool flipImage)
    {
		stbi_set_flip_vertically_on_load(static_cast<int>(flipImage));
		stbi_uc* data = nullptr;
		int width = 0;
		int height = 0;
		int channels = 0;

		data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (data != nullptr)
		{
			imageData.width = static_cast<unsigned int>(width);
			imageData.height = static_cast<unsigned int>(height);

			if (channels == 4)
			{
				imageData.format.layout = PixelLayout::RGBA;
			}
			else if (channels == 3)
			{
				imageData.format.layout = PixelLayout::RGB;
			}

			size_t size = width * height * channels;
			imageData.data.resize(size);
			std::copy(data, data + size, imageData.data.begin());
			stbi_image_free(data);

			//If we got here, then everthing was successful
			return true;
		}

		return false;
    }
}
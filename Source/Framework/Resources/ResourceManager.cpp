#include "ResourceManager.h"
#include "../Audio/Audio.h"
#include "../Audio/AudioTypes.h"
#include "../Debug/Log.h"
#include "../Graphics/Shader.h"
#include "../Graphics/SpriteAtlas.h"
#include "../Graphics/SpriteFont.h"
#include "../Utils/Png/Png.h"
#include "../Utils/Wave/Wave.h"
#include "../Application/Application.h"
#include <json.h>
#include <fstream>
#include <filesystem>


namespace GameDev2D
{
    ResourceManager::ResourceManager(bool autoLoad) :
        m_AudioCache(entt::connect_arg_t<&ResourceManager::LoadWaveData>()),
        m_TextureCache(entt::connect_arg_t<&ResourceManager::LoadTexure>()),
        m_ShaderCache(entt::connect_arg_t<&ResourceManager::LoadShader>()),
        m_FontCache(entt::connect_arg_t<&ResourceManager::LoadFont>()),
        m_AtlasCache(entt::connect_arg_t<&ResourceManager::LoadAtlas>())
    {
        //Local variables
        const unsigned int numChannels = 1;    //Mono
        const unsigned int duration = 1;       //In seconds;
        const unsigned int sampleRate = 8192;
        const unsigned int frequencyA = 650;
        const unsigned int frequencyB = 450;
        const unsigned int frames = sampleRate * duration;

        //Create the WaveData object
        Ref<WaveData> defaultAudio = CreateRef<WaveData>();

        //Initialize the wave format struct
        defaultAudio->waveFormat.wFormatTag = WAVE_FORMAT_PCM;
        defaultAudio->waveFormat.nChannels = numChannels;
        defaultAudio->waveFormat.nSamplesPerSec = sampleRate;
        defaultAudio->waveFormat.nAvgBytesPerSec = sampleRate * sizeof(short);
        defaultAudio->waveFormat.nBlockAlign = sizeof(short);
        defaultAudio->waveFormat.wBitsPerSample = 16; //16 bits in a short
        defaultAudio->waveFormat.cbSize = 0;
        defaultAudio->data = (void*)new short[frames];

        //Fill the data array with a tone generated from a sine wave
        for (unsigned int i = 0; i < frames; i++)
        {
            unsigned int frequency = i < frames / 2 ? frequencyA: frequencyB;
            ((short*)defaultAudio->data)[i] = (short)(SHRT_MAX * sin((2 * M_PI * frequency) / sampleRate * i));
        }

        //Fill the audio buffer struct
        defaultAudio->buffer.AudioBytes = frames * sizeof(short);
        defaultAudio->buffer.pAudioData = (BYTE*)defaultAudio->data;
        defaultAudio->buffer.PlayBegin = 0;
        defaultAudio->buffer.PlayLength = 0;
        m_AudioCache.SetDefaultResource(defaultAudio);

        //Local constants
        const int placeHolderSize = 64;  //Width and Height
        const int colorSize = 4;  //RGBA
        int checkerSize = placeHolderSize >> 1;
        int rowBytes = placeHolderSize * colorSize;
        Color colorA(0x555555a8);
        Color colorB(0xbbbbbba8);

        //Create the default texture object
        PixelFormat format(PixelLayout::RGBA, PixelSize::UnsignedByte);
        ImageData imageData(format, placeHolderSize, placeHolderSize);

        const unsigned int dataSize = colorSize * placeHolderSize * placeHolderSize;
        imageData.data.resize(dataSize);

        //Cycle through and set the checkered board pattern
        int index = 0;
        for (int i = 0; i < checkerSize; ++i)
        {
            for (int j = 0; j < checkerSize; ++j)
            {
                //Red
                imageData.data[index] = imageData.data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.r * 255.0f);
                imageData.data[index + checkerSize * colorSize] = imageData.data[index + checkerSize * rowBytes] = (unsigned char)(colorB.r * 255.0f);
                index++;

                //Green
                imageData.data[index] = imageData.data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.g * 255.0f);
                imageData.data[index + checkerSize * colorSize] = imageData.data[index + checkerSize * rowBytes] = (unsigned char)(colorB.g * 255.0f);
                index++;

                //Blue
                imageData.data[index] = imageData.data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.b * 255.0f);
                imageData.data[index + checkerSize * colorSize] = imageData.data[index + checkerSize * rowBytes] = (unsigned char)(colorB.b * 255.0f);
                index++;

                //Alpha
                imageData.data[index] = imageData.data[index + checkerSize * (colorSize + rowBytes)] = (unsigned char)(colorA.a * 255.0f);
                imageData.data[index + checkerSize * colorSize] = imageData.data[index + checkerSize * rowBytes] = (unsigned char)(colorB.a * 255.0f);
                index++;
            }
            index += checkerSize * colorSize;
        }

        m_TextureCache.SetDefaultResource(CreateRef<Texture>(imageData));
        m_TextureCache.ResourceLoadedSlot.connect<&ResourceManager::OnTextureLoaded>(this);

        std::unordered_map<std::string, Rect> defaultMap;
        m_AtlasCache.SetDefaultResource(CreateRef<AtlasMap>(defaultMap));

        Resource resource("OpenSans-CondBold_32", "png", "Fonts");
        Ref<Font> defaultFont;
        LoadFont(resource, defaultFont);
        m_FontCache.SetDefaultResource(defaultFont);

        if (autoLoad)
        {
            std::vector<std::string> ignoreExtensions;
            std::vector<Resource> audioResources = AutoLoadResource("Audio", ignoreExtensions);
            for (size_t i = 0; i < audioResources.size(); i++)
            {
                GetAudioCache().Load(audioResources.at(i));
            }

            ignoreExtensions.clear();
            ignoreExtensions.push_back("json");
            std::vector<Resource> fontResources = AutoLoadResource("Fonts", ignoreExtensions);
            for (size_t i = 0; i < fontResources.size(); i++)
            {
                GetFontCache().Load(fontResources.at(i));
            }

            ignoreExtensions.clear();
            ignoreExtensions.push_back("json");
            std::vector<Resource> imagesResources = AutoLoadResource("Images", ignoreExtensions);
            for (size_t i = 0; i < imagesResources.size(); i++)
            {
                GetTextureCache().Load(imagesResources.at(i));
            }

            ignoreExtensions.clear();
            ignoreExtensions.push_back("json");
            std::vector<Resource> splashResources = AutoLoadResource("Splash", ignoreExtensions);
            for (size_t i = 0; i < splashResources.size(); i++)
            {
                GetTextureCache().Load(splashResources.at(i));
            }

            ignoreExtensions.clear();
            ignoreExtensions.push_back("vsh");
            ignoreExtensions.push_back("fsh");
            ignoreExtensions.push_back("gsh");
            std::vector<Resource> shaderResources = AutoLoadResource("Shaders", ignoreExtensions);
            for (size_t i = 0; i < shaderResources.size(); i++)
            {
                GetShaderCache().Load(shaderResources.at(i));
            }
        }
        else
        {
            Resource spriteRenderer("SpriteRenderer", "glsl", "Shaders");
            GetShaderCache().Load(spriteRenderer, SPRITE_RENDERER_SHADER_KEY);

            Resource lineRenderer("LineRenderer", "glsl", "Shaders");
            GetShaderCache().Load(lineRenderer, LINE_RENDERER_SHADER_KEY);
        }
    }

    ResourceCache<WaveData>& ResourceManager::GetAudioCache()
    {
        return m_AudioCache;
    }

    ResourceCache<Texture>& ResourceManager::GetTextureCache()
    {
        return m_TextureCache;
    }

    ResourceCache<Shader>& ResourceManager::GetShaderCache()
    {
        return m_ShaderCache;
    }

    ResourceCache<Font>& ResourceManager::GetFontCache()
    {
        return m_FontCache;
    }

    ResourceCache<AtlasMap>& ResourceManager::GetAtlasCache()
    {
        return m_AtlasCache;
    }

    void ResourceManager::OnTextureLoaded(Texture* texture, const Resource& resource)
    {
        //When a texture is loaded, check to see if it as a json file, that would 
        //indicate that its a sprite atlas, and it would also have to be loaded
        Resource atlasResource(resource.filename, "json", resource.directory);
        std::string path = Application::Get().GetFileSystem().GetPathForResource(atlasResource);
        if (Application::Get().GetFileSystem().DoesFileExistAtPath(path))
        {
            GetAtlasCache().Load(atlasResource);
        }
    }

    std::vector<Resource> ResourceManager::AutoLoadResource(const std::string& directory, std::vector<std::string>& ignoreExtensions)
    {
        std::vector<Resource> resources;

        std::string path = Application::Get().GetFileSystem().GetApplicationDirectory() + "\\Assets\\" + directory;
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            std::string filename(entry.path().stem().u8string());
            std::string extension(entry.path().extension().u8string());
            extension.erase(extension.begin()); //remove the .

            bool addResource = true;
            for (unsigned int i = 0; i < ignoreExtensions.size(); i++)
            {
                if (ignoreExtensions.at(i) == extension)
                {
                    addResource = false;
                }
            }

            if (addResource)
            {
                resources.push_back(Resource(filename, extension, directory));
            }
        }

        return resources;
    }

    void ResourceManager::LoadWaveData(const Resource& resource, Ref<WaveData>& waveData)
    {
        if (!resource.IsValid())
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load wave file, the filename had a length of 0");
            return;
        }

        std::string path = Application::Get().GetFileSystem().GetPathForResource(resource);
        if (Application::Get().GetFileSystem().DoesFileExistAtPath(path))
        {
            waveData = CreateRef<WaveData>();
            if (Wave::LoadFromPath(path, waveData) == false)
            {
                waveData.reset();
                Log::Error(false, LogVerbosity::Resources, "[Resource Manager] *** An error occured while loading wave file with filename: %s.wav ***", resource.filename.c_str());
            }
        }
        else
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] *** Failed to load wave file with filename: %s.wav, it doesn't exist ***", resource.filename.c_str());
        }
    }

    void ResourceManager::LoadTexure(const Resource& resource, Ref<Texture>& texture)
    {
        if (!resource.IsValid())
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load texture, the filename had a length of 0");
            return;
        }

        std::string path = Application::Get().GetFileSystem().GetPathForResource(resource);
        if (Application::Get().GetFileSystem().DoesFileExistAtPath(path))
        {
            ImageData imageData;
            if (Png::LoadFromPath(path, imageData))
            {
                texture = CreateRef<Texture>(imageData);
            }
            else
            {
                Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load texture : %s.%s", resource.filename.c_str(), resource.extension.c_str());
            }
        }
    }

    void ResourceManager::LoadShader(const Resource& resource, Ref<Shader>& shader)
    {
        if (!resource.IsValid())
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load shader, the filename had a length of 0");
            return;
        }

        std::string path = Application::Get().GetFileSystem().GetPathForResource(resource);
        std::string source = ReadFile(path);
        if (source.length() > 0)
        {
            std::unordered_map<ShaderType, std::string> shaderSources = PreProcessShader(source);
            shader = CreateRef<Shader>(shaderSources[ShaderType::Vertex], shaderSources[ShaderType::Fragment], shaderSources[ShaderType::Geometry]);
        }
        else
        {
            Log::Error(true, LogVerbosity::Resources, "[Resource Manager] Failed to load Shader : %s.%s", resource.filename.c_str(), resource.extension.c_str());
        }
    }

    void ResourceManager::LoadFont(const Resource& resource, Ref<Font>& font)
    {
        if (!resource.IsValid())
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load font, the filename had a length of 0");
            return;
        }

        std::string jsonPath = Application::Get().GetFileSystem().GetPathForResource(resource.filename, "json", resource.directory);
        std::string jsonData = ReadFile(jsonPath);

        if (jsonData.length() > 0)
        {
            FontData fontData;
            if (ParseFontData(jsonData, fontData))
            {
                //Get the path for the texture
                std::string path = Application::Get().GetFileSystem().GetPathForResource(resource);

                //Does the image exist at the path
                if (Application::Get().GetFileSystem().DoesFileExistAtPath(path))
                {
                    //Attempt to load the png image and store its texture data in the TextureData struct
                    ImageData imageData;
                    if (Png::LoadFromPath(path, imageData))
                    {
                        font = CreateRef<Font>(imageData, fontData);
                    }
                    else
                    {
                        Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load font texture: %s", resource.filename.c_str());
                    }
                }
            }
            else
            {
                Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to parse json font data: %s", resource.filename.c_str());
            }
        }
        else
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to load font: %s", resource.filename.c_str());
        }
    }

    void ResourceManager::LoadAtlas(const Resource& resource, Ref<AtlasMap>& atlas)
    {
        if (!resource.IsValid())
        {
            Log::Error(false, LogVerbosity::Resources, "[Resource Manager] Failed to texture atlas, the filename had a length of 0");
            return;
        }

        std::string jsonPath = Application::Get().GetFileSystem().GetPathForResource(resource);
        std::string jsonData = ReadFile(jsonPath);

        if (jsonData.length() > 0)
        {
            std::unordered_map<std::string, Rect> atlasMap;
            if (ParseAtlasData(jsonData, atlasMap))
            {
                atlas = CreateRef<AtlasMap>(atlasMap);
            }
        }
    }

    std::string ResourceManager::ReadFile(const std::string& path)
    {
        std::string result;
        std::ifstream in(path, std::ios::in | std::ios::binary);
        if (in)
        {
            in.seekg(0, std::ios::end);
            size_t size = static_cast<size_t>(in.tellg());
            if (size != -1)
            {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            }
            else
            {
                Log::Error(false, LogVerbosity::Resources, "Could not read from file %s", path.c_str());
            }
        }
        else
        {
            Log::Error(false, LogVerbosity::Resources, "Could not open file %s", path.c_str());
        }

        return result;
    }

    std::unordered_map<ShaderType, std::string> ResourceManager::PreProcessShader(const std::string& source)
    {
        std::unordered_map<ShaderType, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            assert(eol != std::string::npos); //Syntax error
            
            size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            assert(Shader::GetShaderType(type) != ShaderType::Unknown); //Invalid shader type specified

            size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            assert(nextLinePos != std::string::npos); //Syntax error
            pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

            shaderSources[Shader::GetShaderType(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
        }

        if (shaderSources.find(ShaderType::Geometry) == shaderSources.end())
        {
            shaderSources[ShaderType::Geometry] = "";
        }

        return shaderSources;
    }

    bool ResourceManager::ParseFontData(const std::string& jsonData, FontData& fontData)
    {
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(jsonData, root, false) == true)
        {
            //Create the FontData object
            fontData.name = root["file"].asString();
            fontData.characterSet = root["characterSet"].asString();
            fontData.size = root["size"].asUInt();
            fontData.baseline = root["baseline"].asUInt();
            fontData.lineHeight = root["lineHeight"].asUInt();

            //Get the glyphs
            const Json::Value glyphs = root["glyphs"];

            //Cycle through the frames and parse the frame data
            for (unsigned int index = 0; index < glyphs.size(); ++index)
            {
                //Safety check the filename
                if (glyphs[index]["character"].isString() == true)
                {
                    //Get the character
                    std::string characterStr = std::string(glyphs[index]["character"].asString());
                    char character = characterStr.at(0);

                    fontData.glyphData[character].advanceX = (unsigned char)glyphs[index]["advanceX"].asInt();
                    fontData.glyphData[character].bearingX = (unsigned char)glyphs[index]["bearingX"].asInt();
                    fontData.glyphData[character].bearingY = (unsigned char)glyphs[index]["bearingY"].asInt();

                    //Load the frame
                    Json::Value frameValue = glyphs[index]["frame"];
                    unsigned int x = frameValue["x"].asUInt();
                    unsigned int y = frameValue["y"].asUInt();
                    unsigned int width = frameValue["w"].asUInt();
                    unsigned int height = frameValue["h"].asUInt();

                    //Set the frame
                    fontData.glyphData[character].frame = Rect(Vector2((float)x, (float)y), Vector2((float)width, (float)height));
                }
            }

            //The parsing was successful
            return true;
        }

        //The parsing failed
        return false;
    }

    bool ResourceManager::ParseAtlasData(const std::string& jsonData, std::unordered_map<std::string, Rect>& atlasMap)
    {
        //Parse the json data
        Json::Value root;
        Json::Reader reader;
        if (reader.parse(jsonData, root, false) == true)
        {
            //Get the frames
            const Json::Value frames = root["frames"];

            //Cycle through the frames and parse the frame data
            for (unsigned int index = 0; index < frames.size(); ++index)
            {
                //Safety check the filename
                if (frames[index]["filename"].isString() == true)
                {
                    //Get the atlas key
                    std::string atlasKey = std::string(frames[index]["filename"].asString());

                    //Load the source frame
                    Json::Value frameValue = frames[index]["frame"];

                    //Get the values from the frame
                    unsigned int x = frameValue["x"].asUInt();
                    unsigned int y = frameValue["y"].asUInt();
                    unsigned int width = frameValue["w"].asUInt();
                    unsigned int height = frameValue["h"].asUInt();

                    //Return the source frame
                    Rect frame(Vector2((float)x, (float)y), Vector2((float)width, (float)height));
                    atlasMap[atlasKey] = frame;
                }
            }

            //The parsing was successful
            return true;
        }

        //The parsing failed
        return false;
    }
}
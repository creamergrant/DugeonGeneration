#pragma once

#include "Resource.h"
#include "ResourceCache.h"
#include "../Audio/AudioTypes.h"
#include "../Graphics/Font.h"
#include "../Graphics/Sprite.h"
#include "../Graphics/Texture.h"
#include "../Graphics/GraphicTypes.h"
#include <unordered_map>
#include <string>


//Shader constants
const std::string SPRITE_RENDERER_SHADER_KEY = "SpriteRenderer";
const std::string LINE_RENDERER_SHADER_KEY = "LineRenderer";
const std::string POINT_RENDERER_SHADER_KEY = "PointRenderer";
const std::string CIRCLE_RENDERER_SHADER_KEY = "CircleRenderer";

namespace GameDev2D
{
    class AtlasMap;
    class Audio;
    class Shader;

    class ResourceManager
    {
    public:
        ResourceManager(bool autoLoad);
        ~ResourceManager() = default;

        ResourceCache<WaveData>& GetAudioCache();
        ResourceCache<Texture>& GetTextureCache();
        ResourceCache<Shader>& GetShaderCache();
        ResourceCache<Font>& GetFontCache();
        ResourceCache<AtlasMap>& GetAtlasCache();

    private:
        void OnTextureLoaded(Texture* texture, const Resource& resource);

        std::vector<Resource> AutoLoadResource(const std::string& directory, std::vector<std::string>& ignoreExtensions);

        static void LoadWaveData(const Resource& resource, Ref<WaveData>& waveData);
        static void LoadTexure(const Resource& resource, Ref<Texture>& texture);
        static void LoadShader(const Resource& resource, Ref<Shader>& shader);
        static void LoadFont(const Resource& resource, Ref<Font>& font);
        static void LoadAtlas(const Resource& resource, Ref<AtlasMap>& atlas);

        static std::string ReadFile(const std::string& path);
        static std::unordered_map<ShaderType, std::string> PreProcessShader(const std::string& source);
        static bool ParseFontData(const std::string& jsonData, FontData& fontData);
        static bool ParseAtlasData(const std::string& jsonData, std::unordered_map<std::string, Rect>& atlasMap);

        //Member variables
        ResourceCache<WaveData> m_AudioCache;
        ResourceCache<Texture> m_TextureCache;
        ResourceCache<Shader> m_ShaderCache;
        ResourceCache<Font> m_FontCache;
        ResourceCache<AtlasMap> m_AtlasCache;
    };

    class AtlasMap
    {
    public:
        AtlasMap(std::unordered_map<std::string, Rect>& atlasMap) :
            m_AtlasMap(atlasMap)
        {}

        Rect Frame(const std::string& aKey) { return m_AtlasMap[aKey]; }

    private:
        std::unordered_map<std::string, Rect> m_AtlasMap;
    };
}

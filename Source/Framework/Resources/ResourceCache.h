#pragma once

#include "Resource.h"
#include <entt.hpp>
#include <string>
#include <map>


namespace GameDev2D
{
    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    class ResourceCache
    {
    public:
        template<auto Candidate>
        ResourceCache(entt::connect_arg_t<Candidate> args) :
            ResourceLoadedSlot(m_ResourceLoadedSignal),
            ResourceUnloadedSlot(m_ResourceUnloadedSignal),
            m_LoadingDelegate(args)
        {}

        ~ResourceCache()
        {
            Clear();
        }

        entt::sink<void(T*, const Resource&)> ResourceLoadedSlot;
        entt::sink<void(T*)> ResourceUnloadedSlot;

        void Load(const Resource& resource, const std::string& identifier = "")
        {
            std::string id = identifier == "" ? resource.filename : identifier;

            if (IsLoaded(id) == false)
            {
                Ref<T> asset;
                m_LoadingDelegate(resource, asset);

                if (asset)
                {
                    m_ResourceLoadedSignal.publish(asset.get(), resource);
                    m_Map[id] = asset;
                }
            }
        }

        void Unload(const std::string& identifier)
        {
            if (IsLoaded(identifier) == true)
            {
                Ref<T> resource = m_Map[identifier];

                //Dispatch an event that the resource was unloaded
                m_ResourceUnloadedSignal.publish(resource.get());

                resource.reset();

                m_Map.erase(identifier);
            }
        }

        bool IsLoaded(const std::string& identifier)
        {
            return m_Map.find(identifier) != m_Map.end();
        }

        T* Get(const std::string& identifier)
        {
            //Set the texture data
            if (IsLoaded(identifier) == true)
            {
                return m_Map[identifier].get();
            }

            //If the resource still isn't loaded, it doesn't exist, return 
            //the default resource instead
            return m_DefaultResource.get();
        }

        Ref<T> GetShared(const std::string& identifier)
        {
            //Set the texture data
            if (IsLoaded(identifier) == true)
            {
                return m_Map[identifier];
            }

            //If the resource still isn't loaded, it doesn't exist, return 
            //the default resource instead
            return m_DefaultResource;
        }

        void Clear()
        {
            m_Map.clear();
        }

        void SetDefaultResource(Ref<T> defaultResource)
        {
            m_DefaultResource = defaultResource;
        }

        T* GetDefaultResource()
        {
            return m_DefaultResource.get();
        }

        std::string GetKey(T* ptr)
        {
            for (auto const& r : m_Map)
            {
                if (r.second.get() == ptr)
                {
                    return r.first;
                }
            }

            return "";
        }

    private:
        entt::sigh<void(T*, const Resource&)> m_ResourceLoadedSignal;
        entt::sigh<void(T*)> m_ResourceUnloadedSignal;
        entt::delegate<void(const Resource&, Ref<T>&)> m_LoadingDelegate;
        std::unordered_map<std::string, Ref<T>> m_Map;
        Ref<T> m_DefaultResource;
    };
}
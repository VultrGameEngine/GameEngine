#pragma once
#include <types/types.hpp>
#include <rendering/types/shader.h>
#include <rendering/types/texture.h>
#include <rendering/types/mesh.h>
#include <queue>

namespace Vultr
{
    typedef u32 AssetHash;

#define MAX_RESOURCE_CACHE_SIZE 1024

    template <typename T>
    struct ResourceCache
    {
        void alloc_asset(AssetHash asset)
        {
            assert(asset_to_index_map.find(asset) == asset_to_index_map.end() && "Asset already allocated!");

            size_t new_index = len;

            asset_to_index_map[asset] = new_index;
            index_to_asset_map[new_index] = asset;

            len++;
        }

        void free_asset(AssetHash asset)
        {
        }

        std::unordered_map<AssetHash, size_t> asset_to_index_map{};
        std::unordered_map<size_t, AssetHash> index_to_asset_map{};

        ~ResourceCache()
        {
            delete[] cache;
        }

        // Not really sure if this memory usage is the correct decision :/
        T *cache = new T[MAX_RESOURCE_CACHE_SIZE];
        size_t len = 0;
    };

    template <typename T>
    void load_resource(T *resource);

    struct ResourceManager
    {
        ResourceManager() = default;
        ~ResourceManager() = default;

        virtual void incr(AssetHash asset);
        virtual void decr(AssetHash asset);
    };

    // Ok I must admit I went way overboard with the templates but I actually like the API that this creates so I'm keeping it
    template <typename... ResourceTypes>
    struct InternalResourceManager : ResourceManager
    {
        std::tuple<ResourceCache<ResourceTypes>...> resource_caches;

        InternalResourceManager()
        {
        }

        ~InternalResourceManager()
        {
        }

        void garbage_collect()
        {
        }

        void incr(AssetHash asset) override
        {
        }

        void decr(AssetHash asset) override
        {
        }
    };

    template <typename T>
    struct Resource
    {
        ResourceManager *rm = nullptr;
        AssetHash asset = 0;
        Resource(AssetHash asset, ResourceManager *rm)
        {
        }

        ~Resource()
        {
            rm->decr(asset);
        }

        T *operator*()
        {
        }
    };
} // namespace Vultr

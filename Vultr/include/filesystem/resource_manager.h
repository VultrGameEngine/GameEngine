#pragma once
#include <types/types.hpp>
#include <queue>

namespace Vultr
{
    typedef u32 AssetHash;

#define MAX_RESOURCE_CACHE_SIZE 1024

    template <typename T>
    void load_resource(T *resource);

    template <typename T>
    void free_resource(T *resource);

    struct IResourceQueueItem
    {
    };

    template <typename T>
    struct ResourceQueueItem : IResourceQueueItem
    {
        AssetHash hash;

        ResourceQueueItem(AssetHash hash)
        {
            this->hash = hash;
        }

        ~ResourceQueueItem()
        {
        }
    };

    template <typename T>
    struct ResourceQueue
    {
        T *items = nullptr;
        size_t len = 0;
        std::mutex queue_mutex;

        ResourceQueue()
        {
        }

        ~ResourceQueue()
        {
            if (items != nullptr)
            {
                free(items);
            }
        }

        ResourceQueue(const ResourceQueue &) = delete;
        void operator=(const ResourceQueue &) = delete;

        T *top()
        {
            queue_mutex.lock();
            queue_mutex.unlock();
        }

        void resize()
        {
            items = static_cast<T *>(realloc(items, sizeof(T) * len));
        }

        void push(T *item)
        {
            queue_mutex.lock();
            if (len == 0)
            {
                assert(items == nullptr && "Items already allocated for some reason?");
                len++;
                items = static_cast<T *>(malloc(sizeof(T)));
            }
            else
            {
                len++;
                resize();
                for (s32 i = 0; i < len - 1; i++)
                {
                    items[i + 1] = items[i];
                }
            }

            items[len - 1] = *item;
            queue_mutex.unlock();
        }

        T pop()
        {
            queue_mutex.lock();
            T copy = items[len - 1];
            if (len == 1)
            {
                len--;
                free(items);
                items = nullptr;
            }
            else
            {
                len--;
                resize();
            }

            queue_mutex.unlock();

            return copy;
        }
    };

    template <typename T>
    struct ResourceCache
    {
        ResourceCache() = default;

        ~ResourceCache()
        {
            for (s32 i = 0; i < len; i++)
            {
                free_resource(&cache[i]);
            }
            delete[] cache;
        }

        void operator=(const ResourceCache<T> &other) = delete;
        ResourceCache(const ResourceCache<T> &other) = delete;

        template <typename InvalidType>
        void incr_asset(AssetHash asset, InvalidType **asset_load){};
        void incr_asset(AssetHash asset, T **asset_load)
        {
            if (asset_to_index_map.find(asset) != asset_to_index_map.end())
            {
                asset_counter[asset]++;
            }
            else
            {
                size_t new_index = len;

                asset_to_index_map[asset] = new_index;
                index_to_asset_map[new_index] = asset;
                asset_counter[asset] = 1;
                asset_loaded[asset] = false;

                len++;
            }
            *asset_load = &cache[asset_to_index_map[asset]];
        }

        template <typename RequestedType>
        void decr_asset(AssetHash asset)
        {
            if (!std::is_same<T, RequestedType>::value)
                return;

            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to remove nonexistent asset");

            auto count = asset_counter[asset]--;
            if (count < 1)
            {
                asset_free_queue.push(asset);
            }
        }

        template <typename RequestedType>
        void load_asset(AssetHash asset, RequestedType *data){};
        void load_asset(AssetHash asset, T *data)
        {
            asset_loaded[asset] = true;
            cache[asset_to_index_map[asset]] = *data;
        }

        template <typename RequestedType>
        void get_asset(AssetHash hash, RequestedType **asset) const {};
        void get_asset(AssetHash hash, T **asset) const
        {
            assert(has_asset(hash) && "Retreive nonexistent asset!");
            assert(is_asset_loaded(hash) && "Asset not loaded!");

            *asset = &cache[asset_to_index_map.at(hash)];
        }

        template <typename RequestedType>
        void has_asset(AssetHash asset, bool *has_asset) const
        {
            if (!std::is_same<T, RequestedType>::value)
                return;

            *has_asset = *has_asset || asset_to_index_map.find(asset) != asset_to_index_map.end();
        }

        template <typename RequestedType>
        void is_asset_loaded(AssetHash asset, bool *loaded) const
        {
            if (!std::is_same<T, RequestedType>::value)
                return;

            assert(has_asset(asset) && "Nonexistent asset!");
            *loaded = *loaded || asset_loaded.at(asset);
        }

        void garbage_collect()
        {
            while (!asset_free_queue.empty())
            {
                // Get the asset
                AssetHash asset = asset_free_queue.front();

                // Get the index for the specified asset
                size_t index_of_removed_asset = asset_to_index_map[asset];

                // Get the index of the last element of the component array
                size_t index_of_last_element = len - 1;

                // Free the resource
                free_resource(&cache[index_of_removed_asset]);

                // Move the last element of the component array into the removed asset's index
                cache[index_of_removed_asset] = cache[index_of_last_element];

                // Update the maps for the newly moved element
                AssetHash asset_of_last_element = index_to_asset_map[index_of_last_element];
                asset_to_index_map[asset_of_last_element] = index_of_removed_asset;
                index_to_asset_map[index_of_removed_asset] = asset_of_last_element;

                // Remove the asset requested from the maps
                asset_to_index_map.erase(asset);
                index_to_asset_map.erase(index_of_last_element);
                asset_counter.erase(asset);
                asset_loaded.erase(asset);

                len--;
                asset_free_queue.pop();
            }
        }

        std::unordered_map<AssetHash, size_t> asset_to_index_map{};
        std::unordered_map<size_t, AssetHash> index_to_asset_map{};
        std::unordered_map<AssetHash, u32> asset_counter{};
        std::unordered_map<AssetHash, bool> asset_loaded{};
        std::queue<AssetHash> asset_free_queue{};

        // Not really sure if this memory usage is the correct decision :/
        T *cache = new T[MAX_RESOURCE_CACHE_SIZE];
        size_t len = 0;
    };

    // Ok I must admit I went way overboard with the templates but I actually like the API that this creates so I'm keeping it
    template <typename... ResourceType>
    struct InternalResourceManager
    {
        std::tuple<ResourceCache<ResourceType>...> resource_caches;
        std::mutex resource_mutex;
        ResourceQueue<IResourceQueueItem *> queue;

        InternalResourceManager() = default;

        ~InternalResourceManager() = default;

        template <typename T>
        void incr(AssetHash hash)
        {
            resource_mutex.lock();
            T *asset = nullptr;
            std::apply([&](auto &...cache) { (..., cache.incr_asset(hash, &asset)); }, resource_caches);

            assert(asset != nullptr && "Something went wrong, failed to allocate asset!");
            resource_mutex.unlock();
        }

        template <typename T>
        void decr(AssetHash hash)
        {
            resource_mutex.lock();
            std::apply([&](auto &...cache) { (..., cache.template decr_asset<T>(hash)); }, resource_caches);
            resource_mutex.unlock();
        }

        template <typename T>
        T *get_asset(AssetHash hash)
        {
            resource_mutex.lock();
            T *asset = nullptr;
            std::apply([&](auto &...cache) { (..., cache.get_asset(hash, &asset)); }, resource_caches);

            resource_mutex.unlock();
            return asset;
        }
        template <typename T>
        bool has_asset(AssetHash hash)
        {
            resource_mutex.lock();
            bool has_asset = false;
            std::apply([&](auto &...cache) { (..., cache.template has_asset<T>(hash, &has_asset)); }, resource_caches);

            resource_mutex.unlock();
            return has_asset;
        }

        template <typename T>
        bool is_asset_loaded(AssetHash hash)
        {
            resource_mutex.lock();
            bool loaded = false;
            std::apply([&](auto &...cache) { (..., cache.template is_asset_loaded<T>(hash, &loaded)); }, resource_caches);

            resource_mutex.unlock();
            return loaded;
        }

        void garbage_collect(AssetHash hash)
        {
            resource_mutex.lock();
            std::apply([&](auto &...cache) { (..., cache.garbage_collect()); }, resource_caches);

            resource_mutex.unlock();
        }
    };
} // namespace Vultr

#pragma once
#include <filesystem/virtual_filesystem.h>
#include <queue.h>

// TODO: This entire template system is extremely ugly to me. If there is a better way to do this I need to figure out a replacement. This is mostly unmaintainable, even though it does work.
namespace Vultr
{
    typedef u32 AssetHash;

#define MAX_RESOURCE_CACHE_SIZE 1024

    template <typename T>
    void load_resource(const VirtualFilesystem *vfs, AssetHash hash, T *resource);

    template <typename T>
    void free_resource(T *resource);

    struct IResourceQueueItem
    {
        virtual void load(){};
        virtual ~IResourceQueueItem() = default;
    };

    template <typename T>
    struct ResourceQueueItem : IResourceQueueItem
    {
        typedef std::function<void(AssetHash, T *)> LoadCallback;
        AssetHash hash;
        LoadCallback load_callback;
        const VirtualFilesystem *vfs = nullptr;

        ResourceQueueItem(const VirtualFilesystem *vfs, AssetHash hash, const LoadCallback &load_callback)
        {
            this->hash = hash;
            this->load_callback = load_callback;
            this->vfs = vfs;
        }

        void load()
        {
            T res;
            load_resource<T>(vfs, hash, &res);
            load_callback(hash, &res);
        }

        ~ResourceQueueItem()
        {
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
        void incr_asset(const VirtualFilesystem *, AssetHash, ResourceQueueItem<InvalidType> **){};
        void incr_asset(const VirtualFilesystem *vfs, AssetHash asset, ResourceQueueItem<T> **resource_item)
        {
            cache_mutex.lock();
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
            std::function<void(AssetHash, T *)> cb = [&](AssetHash hash, T *data) {
                bool has = false;
                has_asset<T>(hash, &has);
                if (has)
                {
                    load_asset(hash, data);
                }
            };
            *resource_item = new ResourceQueueItem<T>(vfs, asset, cb);
            cache_mutex.unlock();
        }

        template <typename RequestedType>
        void decr_asset(AssetHash asset)
        {
            cache_mutex.lock();
            if (!std::is_same<T, RequestedType>::value)
                return;

            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to remove nonexistent asset");

            auto count = asset_counter[asset]--;
            if (count < 1)
            {
                asset_free_queue.push(&asset);
            }
            cache_mutex.unlock();
        }

        template <typename RequestedType>
        void load_asset(AssetHash asset, RequestedType *data){};
        void load_asset(AssetHash asset, T *data)
        {
            cache_mutex.lock();
            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to load nonexistent asset!");
            asset_loaded[asset] = true;
            cache[asset_to_index_map[asset]] = *data;
            cache_mutex.unlock();
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
            cache_mutex.lock();
            while (!asset_free_queue.empty())
            {
                // Get the asset
                AssetHash asset = *asset_free_queue.front();

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
            cache_mutex.unlock();
        }

        std::unordered_map<AssetHash, size_t> asset_to_index_map{};
        std::unordered_map<size_t, AssetHash> index_to_asset_map{};
        std::unordered_map<AssetHash, u32> asset_counter{};
        std::unordered_map<AssetHash, bool> asset_loaded{};
        vtl::Queue<AssetHash> asset_free_queue{};
        vtl::mutex cache_mutex;

        // Not really sure if this memory usage is the correct decision :/
        T *cache = new T[MAX_RESOURCE_CACHE_SIZE];
        size_t len = 0;
    };

    template <typename... ResourceType>
    struct InternalResourceManager
    {
        std::tuple<ResourceCache<ResourceType>...> resource_caches;
        vtl::Queue<IResourceQueueItem *> queue;

        InternalResourceManager() = default;

        ~InternalResourceManager() = default;

        template <typename T>
        void incr(const VirtualFilesystem *vfs, AssetHash hash)
        {
            ResourceQueueItem<T> *item = nullptr;
            std::apply([&](auto &...cache) { (..., cache.incr_asset(vfs, hash, &item)); }, resource_caches);

            assert(item != nullptr && "Something went wrong, failed to allocate asset!");

            // ??? For some reason c++ really wants me to put this in a temp pointer
            IResourceQueueItem *_ = static_cast<IResourceQueueItem *>(item);
            queue.push(&_);
        }

        template <typename T>
        void decr(AssetHash hash)
        {
            std::apply([&](auto &...cache) { (..., cache.template decr_asset<T>(hash)); }, resource_caches);
        }

        template <typename T>
        T *get_asset(AssetHash hash)
        {
            T *asset = nullptr;
            std::apply([&](auto &...cache) { (..., cache.get_asset(hash, &asset)); }, resource_caches);

            return asset;
        }
        template <typename T>
        bool has_asset(AssetHash hash)
        {
            bool has_asset = false;
            std::apply([&](auto &...cache) { (..., cache.template has_asset<T>(hash, &has_asset)); }, resource_caches);

            return has_asset;
        }

        template <typename T>
        bool is_asset_loaded(AssetHash hash)
        {
            bool loaded = false;
            std::apply([&](auto &...cache) { (..., cache.template is_asset_loaded<T>(hash, &loaded)); }, resource_caches);

            return loaded;
        }

        void garbage_collect(AssetHash hash)
        {
            std::apply([&](auto &...cache) { (..., cache.garbage_collect()); }, resource_caches);
        }
    };
} // namespace Vultr

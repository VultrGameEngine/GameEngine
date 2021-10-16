#pragma once
#include <filesystem/virtual_filesystem.h>
#include <queue.h>

// TODO: This entire template system is extremely ugly to me. If there is a better way to do this I need to figure out a replacement. This is mostly unmaintainable, even though it does work.
namespace Vultr
{
#define MAX_RESOURCE_CACHE_SIZE 1024

    // Any cleanup that is required (such as moving the memory onto the GPU) on the main thread
    template <typename T>
    void finalize_resource(VFileHandle file, void *resource_manager);

    template <typename T>
    void free_resource(T *resource);

    struct IResourceFinalizeItem
    {
        VFileHandle file;
        virtual void finalize(void *resource_manager){};
        virtual ~IResourceFinalizeItem() = default;
    };

    template <typename T>
    struct ResourceFinalizeItem : IResourceFinalizeItem
    {

        ResourceFinalizeItem(VFileHandle file)
        {
            this->file = file;
        }

        void finalize(void *resource_manager) override
        {
            finalize_resource<T>(file, resource_manager);
        }

        ~ResourceFinalizeItem(){};
    };

    // Load file data asynchronously on separate thread
    template <typename T>
    ResourceFinalizeItem<T> *load_resource(const VirtualFilesystem *vfs, VFileHandle file, T *resource);

    struct IResourceLoadItem
    {
        VFileHandle file;
        virtual IResourceFinalizeItem *load()
        {
            return nullptr;
        }

        virtual ~IResourceLoadItem(){};
    };

    template <typename T>
    struct ResourceLoadItem : IResourceLoadItem
    {
        typedef std::function<void(VFileHandle, T *)> LoadCallback;
        LoadCallback load_callback;
        const VirtualFilesystem *vfs = nullptr;

        ResourceLoadItem(const VirtualFilesystem *vfs, VFileHandle file, const LoadCallback &load_callback)
        {
            this->file = file;
            this->load_callback = load_callback;
            this->vfs = vfs;
        }

        IResourceFinalizeItem *load() override
        {
            T res;
            auto *finalize_res = load_resource<T>(vfs, file, &res);
            load_callback(file, &res);

            if (finalize_res == nullptr)
            {
                return nullptr;
            }
            else
            {
                return static_cast<IResourceFinalizeItem *>(finalize_res);
            }
        }

        ~ResourceLoadItem() = default;
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

        ResourceLoadItem<T> *incr_asset(const VirtualFilesystem *vfs, VFileHandle asset)
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
            std::function<void(VFileHandle, T *)> cb = [&](VFileHandle file, T *data) {
                if (has_asset(file))
                {
                    load_asset(file, data);
                }
            };

            cache_mutex.unlock();

            if (asset_loaded.at(asset))
            {
                return nullptr;
            }
            else
            {
                return new ResourceLoadItem<T>(vfs, asset, cb);
            }
        }

        void decr_asset(VFileHandle asset)
        {
            cache_mutex.lock();

            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to remove nonexistent asset");

            auto count = asset_counter[asset]--;
            if (count < 1)
            {
                asset_free_queue.push(&asset);
            }

            cache_mutex.unlock();
        }

        void load_asset(VFileHandle asset, T *data)
        {
            cache_mutex.lock();
            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to load nonexistent asset!");
            asset_loaded[asset] = true;
            cache[asset_to_index_map[asset]] = *data;
            cache_mutex.unlock();
        }

        T *get_asset(VFileHandle file) const
        {
            assert(has_asset(file) && "Retreive nonexistent asset!");
            assert(is_asset_loaded(file) && "Asset not loaded!");

            return &cache[asset_to_index_map.at(file)];
        }

        bool has_asset(VFileHandle asset) const
        {
            return asset_to_index_map.find(asset) != asset_to_index_map.end();
        }

        bool is_asset_loaded(VFileHandle asset) const
        {
            assert(has_asset(asset) && "Nonexistent asset!");
            return asset_loaded.at(asset);
        }

        void garbage_collect()
        {
            cache_mutex.lock();
            while (!asset_free_queue.empty())
            {
                // Get the asset
                VFileHandle asset = *asset_free_queue.front();

                // Get the index for the specified asset
                size_t index_of_removed_asset = asset_to_index_map[asset];

                // Get the index of the last element of the component array
                size_t index_of_last_element = len - 1;

                // Free the resource
                free_resource(&cache[index_of_removed_asset]);

                // Move the last element of the component array into the removed asset's index
                cache[index_of_removed_asset] = cache[index_of_last_element];

                // Update the maps for the newly moved element
                VFileHandle asset_of_last_element = index_to_asset_map[index_of_last_element];
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

        std::unordered_map<VFileHandle, size_t> asset_to_index_map{};
        std::unordered_map<size_t, VFileHandle> index_to_asset_map{};
        std::unordered_map<VFileHandle, u32> asset_counter{};
        std::unordered_map<VFileHandle, bool> asset_loaded{};
        vtl::Queue<VFileHandle> asset_free_queue{};
        vtl::mutex cache_mutex;

        // Not really sure if this memory usage is the correct decision :/
        T *cache = new T[MAX_RESOURCE_CACHE_SIZE];
        size_t len = 0;
    };

    template <typename... ResourceType>
    struct InternalResourceManager
    {
        std::tuple<ResourceCache<ResourceType>...> resource_caches;
        vtl::Queue<IResourceLoadItem *> load_queue;
        vtl::Queue<IResourceFinalizeItem *> finalize_queue;
        std::atomic<bool> can_garbage_collect = true;
        vtl::mutex mutex;

        InternalResourceManager() = default;

        ~InternalResourceManager() = default;

        template <typename T>
        void incr(const VirtualFilesystem *vfs, VFileHandle file)
        {
            auto *cache = &std::get<ResourceCache<T>>(resource_caches);
            ResourceLoadItem<T> *item = cache->incr_asset(vfs, file);

            if (item == nullptr)
                return;

            // ??? For some reason c++ really wants me to put this in a temp pointer
            IResourceLoadItem *_ = static_cast<IResourceLoadItem *>(item);
            load_queue.push(&_);
        }

        template <typename T>
        void decr(VFileHandle file)
        {
            auto *cache = &std::get<ResourceCache<T>>(resource_caches);
            cache->decr_asset(file);
        }

        template <typename T>
        T *get_asset(VFileHandle file)
        {
            auto *cache = &std::get<ResourceCache<T>>(resource_caches);

            return cache->get_asset(file);
        }
        template <typename T>
        bool has_asset(VFileHandle file)
        {
            auto *cache = &std::get<ResourceCache<T>>(resource_caches);

            return cache->has_asset();
        }

        template <typename T>
        bool is_asset_loaded(VFileHandle file)
        {
            auto *cache = &std::get<ResourceCache<T>>(resource_caches);

            return cache->is_asset_loaded(file);
        }

        void garbage_collect()
        {
            if (can_garbage_collect)
            {
                (..., ([&]() {
                     auto *cache = &std::get<ResourceCache<ResourceType>>(resource_caches);
                     cache->garbage_collect();
                 })());
            }
        }
    };
} // namespace Vultr

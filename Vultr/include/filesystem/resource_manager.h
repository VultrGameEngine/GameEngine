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
        virtual void finalize(void *resource_manager){};
        virtual ~IResourceFinalizeItem() = default;
    };

    template <typename T>
    struct ResourceFinalizeItem : IResourceFinalizeItem
    {
        VFileHandle file;

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
        virtual IResourceFinalizeItem *load()
        {
            return nullptr;
        }
        virtual ~IResourceLoadItem() = default;
    };

    template <typename T>
    struct ResourceLoadItem : IResourceLoadItem
    {
        typedef std::function<void(VFileHandle, T *)> LoadCallback;
        VFileHandle file;
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
            auto *finalize_res = static_cast<IResourceFinalizeItem *>(load_resource<T>(vfs, file, &res));
            load_callback(file, &res);
            return finalize_res;
        }

        ~ResourceLoadItem() override{};
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
        void incr_asset(const VirtualFilesystem *, VFileHandle, ResourceLoadItem<InvalidType> **){};
        void incr_asset(const VirtualFilesystem *vfs, VFileHandle asset, ResourceLoadItem<T> **resource_item)
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
                bool has = false;
                has_asset<T>(file, &has);
                if (has)
                {
                    load_asset(file, data);
                }
            };
            if (asset_loaded.at(asset))
            {
                *resource_item = nullptr;
            }
            else
            {
                *resource_item = new ResourceLoadItem<T>(vfs, asset, cb);
            }
            cache_mutex.unlock();
        }

        template <typename RequestedType>
        void decr_asset(VFileHandle asset)
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
        void load_asset(VFileHandle asset, RequestedType *data){};
        void load_asset(VFileHandle asset, T *data)
        {
            cache_mutex.lock();
            assert(asset_to_index_map.find(asset) != asset_to_index_map.end() && "Attempting to load nonexistent asset!");
            asset_loaded[asset] = true;
            cache[asset_to_index_map[asset]] = *data;
            cache_mutex.unlock();
        }

        template <typename RequestedType>
        void get_asset(VFileHandle file, RequestedType **asset) const {};
        void get_asset(VFileHandle file, T **asset) const
        {
            assert(has_asset(file) && "Retreive nonexistent asset!");
            assert(is_asset_loaded(file) && "Asset not loaded!");

            *asset = &cache[asset_to_index_map.at(file)];
        }

        template <typename RequestedType>
        void has_asset(VFileHandle asset, bool *has_asset) const
        {
            if (!std::is_same<T, RequestedType>::value)
                return;

            *has_asset = *has_asset || asset_to_index_map.find(asset) != asset_to_index_map.end();
        }

        template <typename RequestedType>
        void is_asset_loaded(VFileHandle asset, bool *loaded) const
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
            ResourceLoadItem<T> *item = nullptr;
            std::apply([&](auto &...cache) { (..., cache.incr_asset(vfs, file, &item)); }, resource_caches);

            if (item == nullptr)
                return;

            // ??? For some reason c++ really wants me to put this in a temp pointer
            IResourceLoadItem *_ = static_cast<IResourceLoadItem *>(item);
            load_queue.push(&_);
        }

        template <typename T>
        void decr(VFileHandle file)
        {
            std::apply([&](auto &...cache) { (..., cache.template decr_asset<T>(file)); }, resource_caches);
        }

        template <typename T>
        T *get_asset(VFileHandle file)
        {
            T *asset = nullptr;
            std::apply([&](auto &...cache) { (..., cache.get_asset(file, &asset)); }, resource_caches);

            return asset;
        }
        template <typename T>
        bool has_asset(VFileHandle file)
        {
            bool has_asset = false;
            std::apply([&](auto &...cache) { (..., cache.template has_asset<T>(file, &has_asset)); }, resource_caches);

            return has_asset;
        }

        template <typename T>
        bool is_asset_loaded(VFileHandle file)
        {
            bool loaded = false;
            std::apply([&](auto &...cache) { (..., cache.template is_asset_loaded<T>(file, &loaded)); }, resource_caches);

            return loaded;
        }

        void garbage_collect()
        {
            if (can_garbage_collect)
            {
                std::apply([&](auto &...cache) { (..., cache.garbage_collect()); }, resource_caches);
            }
        }
    };
} // namespace Vultr

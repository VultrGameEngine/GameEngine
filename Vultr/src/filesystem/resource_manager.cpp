#include <filesystem/resource_manager.h>
#include <filesystem/internal/internal_resource_manager.h>
#include <rendering/types/shader.h>
#include <rendering/types/texture.h>
#include <rendering/types/mesh.h>
#include <rendering/types/internal/internal_shader.h>
#include <rendering/types/internal/internal_texture.h>

namespace Vultr
{
    static bool has_asset(ResourceManager *manager, AssetHash asset)
    {
        return manager->assets.find(asset) != manager->assets.end();
    }

    static Shader *get_shader(ResourceManager *manager, AssetHash asset)
    {
        if (!has_asset(manager, asset))
        {
            return nullptr;
        }
        auto *ref_counted_index = &manager->assets[asset];
        return &manager->internal_shaders[ref_counted_index->index];
    }

    static Texture *get_texture(ResourceManager *manager, AssetHash asset)
    {
        if (!has_asset(manager, asset))
        {
            return nullptr;
        }
        auto *ref_counted_index = &manager->assets[asset];
        return &manager->internal_textures[ref_counted_index->index];
    }

    static Mesh *get_mesh(ResourceManager *manager, AssetHash asset)
    {
        if (!has_asset(manager, asset))
        {
            return nullptr;
        }
        auto *ref_counted_index = &manager->assets[asset];
        return &manager->internal_meshes[ref_counted_index->index];
    }

    void asset_manager_garbage_collect(ResourceManager *manager)
    {
        for (auto &[asset, ref_counted_index] : manager->assets)
        {
            if (ref_counted_index.ref_count == 0)
            {
                switch (ref_counted_index.type)
                {
                        // TODO: Implement
                    case ResourceType::SHADER:
                    {
                        break;
                    }
                    case ResourceType::MESH:
                    {
                        break;
                    }
                    case ResourceType::TEXTURE:
                    {
                        break;
                    }
                }
            }
        }
    }
    static void insert_new_asset(ResourceManager *manager, AssetHash asset, ResourceType asset_type)
    {
        size_t index = 0;

        switch (asset_type)
        {
            case ResourceType::SHADER:
            {
                index = manager->internal_shaders_len++;
                break;
            }
            case ResourceType::MESH:
            {
                index = manager->internal_meshes_len++;
                break;
            }
            case ResourceType::TEXTURE:
            {
                index = manager->internal_textures_len++;
                break;
            }
        }
        manager->assets.insert({
            asset,
            ReferenceCountedIndex(index, asset_type),
        });
    }

    static void internal_resource_manager_incr(ResourceManager *manager, AssetHash asset, ResourceType asset_type)
    {
        if (!has_asset(manager, asset))
        {
            insert_new_asset(manager, asset, asset_type);
        }
        manager->assets[asset].ref_count++;
    }

    void internal_resource_manager_decr(ResourceManager *manager, AssetHash asset)
    {
        assert(has_asset(manager, asset) && "Asset not found!");
        manager->assets[asset].ref_count--;
    }

    template <>
    Resource<Texture>::Resource(AssetHash asset, ResourceManager *rm)
    {
        assert(this->rm != nullptr && "Resource manager not found!");
        assert(this->asset != 0 && "Asset not found!");
        this->rm = rm;
        this->asset = asset;
        internal_resource_manager_incr(rm, asset, ResourceType::TEXTURE);
    }

    template <>
    Texture *Resource<Texture>::operator*()
    {
        return get_texture(rm, asset);
    }

    template <>
    Resource<Mesh>::Resource(AssetHash asset, ResourceManager *rm)
    {
        assert(this->rm != nullptr && "Resource manager not found!");
        assert(this->asset != 0 && "Asset not found!");
        this->rm = rm;
        this->asset = asset;
        internal_resource_manager_incr(rm, asset, ResourceType::MESH);
    }
    template <>
    Mesh *Resource<Mesh>::operator*()
    {
        return get_mesh(rm, asset);
    }

    template <>
    Resource<Shader>::Resource(AssetHash asset, ResourceManager *rm)
    {
        assert(this->rm != nullptr && "Resource manager not found!");
        assert(this->asset != 0 && "Asset not found!");
        this->rm = rm;
        this->asset = asset;
        internal_resource_manager_incr(rm, asset, ResourceType::MESH);
    }
    template <>
    Shader *Resource<Shader>::operator*()
    {
        return get_shader(rm, asset);
    }
} // namespace Vultr

#pragma once
#include <ecs/system/system_provider.hpp>
#include <rendering/models/mesh.h>
#include <memory>
#include <unordered_map>
#include <type_info/type_info.h>

namespace Vultr
{
    namespace MeshLoaderSystem
    {
        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Mesh *> meshes;
        };

        Mesh *get_mesh(const char *path);

        void add_mesh(const char *path, Mesh *mesh);

        Component &get_provider();
    } // namespace MeshLoaderSystem

    template <>
    inline const char *get_struct_name<MeshLoaderSystem::Component>()
    {
        return "MeshLoaderSystem";
    }
} // namespace Vultr

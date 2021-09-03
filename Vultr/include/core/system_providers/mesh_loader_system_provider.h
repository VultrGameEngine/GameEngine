#pragma once
#include <engine.hpp>
#include <ecs/system/system_provider.hpp>
#include <rendering/types/mesh.h>
#include <memory>
#include <unordered_map>
#include <type_info/type_info.h>
#include <filesystem/file.h>

namespace Vultr
{
    namespace MeshLoaderSystem
    {
        struct Component : public SystemProvider
        {
            std::unordered_map<std::string, Mesh> meshes;
        };

        Mesh get_mesh(Engine *e, const ModelSource &source);

        void add_mesh(Engine *e, const ModelSource &source, Mesh mesh);

        Component &get_provider(Engine *e);
    } // namespace MeshLoaderSystem

    template <>
    inline const char *get_struct_name<MeshLoaderSystem::Component>()
    {
        return "MeshLoaderSystem";
    }
} // namespace Vultr

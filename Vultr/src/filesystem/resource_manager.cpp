#include <filesystem/resource_manager.h>
#include <rendering/types/texture.h>
#include <rendering/types/shader.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
    template <>
    void load_resource(Texture *resource)
    {
    }

    template <>
    void free_resource(Texture *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    void load_resource(Shader *resource)
    {
    }

    template <>
    void free_resource(Shader *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    void load_resource(Mesh *resource)
    {
    }

    template <>
    void free_resource(Mesh *resource)
    {
        // TODO: Implement to prevent memory leaks
    }
} // namespace Vultr

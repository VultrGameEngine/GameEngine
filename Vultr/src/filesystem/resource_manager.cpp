#include <filesystem/resource_manager.h>
#include <filesystem/virtual_filesystem.h>
#include <rendering/types/texture.h>
#include <rendering/types/shader.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
    template <>
    void load_resource(const VirtualFilesystem *vfs, AssetHash hash, Texture *resource)
    {
        VFileHandle handle(hash);
        assert(vfs_file_exists(vfs, &handle) && "Cannot load texture, file does not exist!");
        printf("Loading texture %s\n", vfs->file_table_path.at(hash).path);
    }

    template <>
    void free_resource(Texture *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    void load_resource(const VirtualFilesystem *vfs, AssetHash hash, Shader *resource)
    {
        VFileHandle handle(hash);
        assert(vfs_file_exists(vfs, &handle) && "Cannot load shader, file does not exist!");
        printf("Loading shader %s\n", vfs->file_table_path.at(hash).path);
    }

    template <>
    void free_resource(Shader *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    void load_resource(const VirtualFilesystem *vfs, AssetHash hash, Mesh *resource)
    {
        VFileHandle handle(hash);
        assert(vfs_file_exists(vfs, &handle) && "Cannot load model, file does not exist!");
        printf("Loading model %s\n", vfs->file_table_path.at(hash).path);
    }

    template <>
    void free_resource(Mesh *resource)
    {
        // TODO: Implement to prevent memory leaks
    }
} // namespace Vultr

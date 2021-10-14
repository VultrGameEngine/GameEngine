#include <filesystem/resource_manager.h>
#include <filesystem/virtual_filesystem.h>
#include <rendering/types/texture.h>
#include <rendering/types/shader.h>
#include <rendering/types/mesh.h>

namespace Vultr
{
    template <>
    ResourceFinalizeItem<Texture> *load_resource(const VirtualFilesystem *vfs, VFileHandle file, Texture *resource)
    {
        assert(vfs_file_exists(vfs, file) && "Cannot load texture, file does not exist!");
        printf("Loading texture %s\n", vfs->file_table_path.at(file).path);
        return new ResourceFinalizeItem<Texture>(file);
    }

    template <>
    void finalize_resource<Texture>(VFileHandle file, void *resource_manager)
    {
        printf("Finalizing texture on main thread!\n");
    }

    template <>
    void free_resource(Texture *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    ResourceFinalizeItem<Shader> *load_resource(const VirtualFilesystem *vfs, VFileHandle file, Shader *resource)
    {
        assert(vfs_file_exists(vfs, file) && "Cannot load shader, file does not exist!");
        printf("Loading shader %s\n", vfs->file_table_path.at(file).path);
        return new ResourceFinalizeItem<Shader>(file);
    }

    template <>
    void finalize_resource<Shader>(VFileHandle file, void *resource_manager)
    {
        printf("Finalizing shader on main thread!\n");
    }

    template <>
    void free_resource(Shader *resource)
    {
        // TODO: Implement to prevent memory leaks
    }

    template <>
    ResourceFinalizeItem<Mesh> *load_resource(const VirtualFilesystem *vfs, VFileHandle file, Mesh *resource)
    {
        assert(vfs_file_exists(vfs, file) && "Cannot load model, file does not exist!");
        printf("Loading model %s\n", vfs->file_table_path.at(file).path);
        return new ResourceFinalizeItem<Mesh>(file);
    }

    template <>
    void finalize_resource<Mesh>(VFileHandle file, void *resource_manager)
    {
        printf("Finalizing mesh on main thread!\n");
    }

    template <>
    void free_resource(Mesh *resource)
    {
        // TODO: Implement to prevent memory leaks
    }
} // namespace Vultr

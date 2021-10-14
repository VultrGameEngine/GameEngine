#pragma once
#include <types/types.hpp>
#include "file.h"
#include <math/crc32.h>

namespace Vultr
{
    typedef u32 VFileHandle;

    struct InternalVFileStream;
    typedef InternalVFileStream VFileStream;

    struct VirtualFilesystem
    {
        std::unordered_map<VFileHandle, GenericFile> file_table_path;
        Directory resource_directory;

        VirtualFilesystem(const Directory *asset_directory);
        ~VirtualFilesystem();
    };

    bool vfs_get_file(const VirtualFilesystem *vfs, const VFileHandle handle, GenericFile *file);
    bool vfs_file_exists(const VirtualFilesystem *vfs, const VFileHandle handle);
    VFileStream *vfs_open(const VirtualFilesystem *vfs, const VFileHandle handle, const char *mode);
    void vfs_close(VFileStream *stream);
    size_t vfs_read(char *ptr, size_t size, size_t nmemb, VFileStream *stream);

    VFileHandle internal_vfile(u32 hash, const char *path, VirtualFilesystem *vfs);
    VFileHandle VFile(const char *path, VirtualFilesystem *vfs);

} // namespace Vultr

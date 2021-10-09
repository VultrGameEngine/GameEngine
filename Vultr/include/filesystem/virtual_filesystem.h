#pragma once
#include <types/types.hpp>
#include "file.h"
#include <math/crc32.h>

namespace Vultr
{
    struct VFileHandle
    {
        u32 id = 0;
        VFileHandle() = default;
        ~VFileHandle() = default;
        VFileHandle(u32 id);
        VFileHandle(const char *path);
    };

#ifndef USE_FILE_ARCHIVE
    struct InternalVFileStream;
    typedef InternalVFileStream VFileStream;
#endif

#define VFILE(path, size) VFileHandle(CRC32_STR(path), size)

    struct VirtualFilesystem
    {
#ifdef USE_FILE_ARCHIVE
        VultrAssetPackage asset_package;
        FILE *fp = nullptr;

        VirtualFilesystem(const VultrAssetPackage *asset_package);
#else
        std::unordered_map<u32, GenericFile> file_table_path;
        Directory resource_directory;

        VirtualFilesystem(const Directory *asset_directory);
#endif
        ~VirtualFilesystem();
    };

#ifdef USE_FILE_ARCHIVE
    size_t vfs_read(char *ptr, size_t size, size_t nmemb, const VFileHandle *file, const VirtualFilesystem *vfs);
#else
    void reimport_assets(VirtualFilesystem *vfs);

    bool vfs_file_exists(const VirtualFilesystem *vfs, const VFileHandle *handle);
    VFileStream *vfs_open(const VirtualFilesystem *vfs, const VFileHandle *handle, const char *mode);
    void vfs_close(VFileStream *stream);
    size_t vfs_read(char *ptr, size_t size, size_t nmemb, VFileStream *stream);
#endif

} // namespace Vultr

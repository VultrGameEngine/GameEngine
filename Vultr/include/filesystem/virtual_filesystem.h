#pragma once
#include <types/types.hpp>
#include "file.h"
#include <math/crc32.h>
#define _BSD_SOURCE
#include <endian.h>

namespace Vultr
{
    // struct VFileDebugInfo
    // {
    //     u32 id = 0;
    //     u32 size = 0;
    //     char *path = nullptr;
    // };

    // struct VDebugInfo
    // {
    //     u64 vdebug_paths_size = 0;
    //     u32 entries = 0;
    // };

    struct VFile
    {
        u32 id = 0;
        u64 size = 0;
        u64 offset = 0;

        VFile() = default;
        VFile(u32 id, u64 size, u64 offset = 0)
        {
            this->id = id;
            this->size = sizeof(VFile) + size;
            this->offset = offset;
        }

        bool write(u64 loc, FILE *f)
        {
            bool res = fseeko(f, loc, SEEK_SET) == 0;

            u32 _id = htole32(id);
            res = res && fwrite(&_id, sizeof(_id), 1, f) == 1;

            u64 _size = htole64(size);
            res = res && fwrite(&_size, sizeof(_size), 1, f) == 1;

            u64 _offset = htole64(offset);
            res = res && fwrite(&_offset, sizeof(_offset), 1, f) == 1;

            return res;
        }

        bool read(u64 loc, FILE *f)
        {
            bool res = fseeko(f, loc, SEEK_SET) == 0;

            res = res && fread(&id, sizeof(id), 1, f) == 1;
            id = le32toh(id);

            res = res && fread(&size, sizeof(size), 1, f) == 1;
            size = le64toh(size);

            res = res && fread(&offset, sizeof(offset), 1, f) == 1;
            offset = le64toh(offset);

            return res;
        }
    };

#define VFILE(path, size) VFile(CRC32_STR(path), size)

    struct VFileHandle
    {
        u32 id = 0;
        VFileHandle() = default;
        VFileHandle(u32 id);
        VFileHandle(const char *path);
    };

    struct VirtualFilesystem
    {
        static const u32 magic_number = CRC32_STR("VultrAssetPackage");
        VultrAssetPackage asset_package;

        u32 version = 0;
        u64 file_table_offset = 0;
        u64 data_offset = 0;

        std::unordered_map<u32, VFile> file_table;
        std::unordered_map<u32, GenericFile> file_table_path;

        FILE *rfp = nullptr;
        FILE *wfp = nullptr;

        VirtualFilesystem(const VultrAssetPackage *asset_package, bool write);
        ~VirtualFilesystem();
    };

    bool vfcreate(VirtualFilesystem *vfs, VFile *file);
    void vfs_read(char *ptr, size_t size, size_t nmemb, VFileHandle *file);

} // namespace Vultr

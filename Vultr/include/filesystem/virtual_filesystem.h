#pragma once
#include <types/types.hpp>
#include "file.h"
#include <math/crc32.h>
#include <hashtable.h>

namespace Vultr
{

    // struct VFileTable
    // {
    //     u32 vfile_table_size = 0;
    //     u32 entries = 0;
    //     u32 *ids = nullptr;
    //     u32 *offsets = nullptr;
    // };

    // struct VFileDebugInfo
    // {
    //     u32 id = 0;
    //     u32 size = 0;
    //     char *path = nullptr;
    // };

    // struct VDebugInfo
    // {
    //     u32 vdebug_paths_size = 0;
    //     u32 entries = 0;
    // };

    // struct VFile
    // {
    //     u32 id = 0;
    //     u32 size = 0;
    //     char *bytes = nullptr;
    // };

    struct VFile
    {
        u32 id;
        u32 size = 0;
        u32 offset = 0;
    };

    struct VirtualFilesystem
    {
        static const u32 magic_number = CRC32_STR("VultrAssetPackage");
        u32 version = 0;

        VultrAssetPackage asset_package;

        vtl::HashTable<u32, VFile> files;

	FILE *fp = nullptr;

        VirtualFilesystem(const VultrAssetPackage *asset_package);
        ~VirtualFilesystem();
    };

} // namespace Vultr

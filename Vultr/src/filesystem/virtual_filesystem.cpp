#include <filesystem/virtual_filesystem.h>

namespace Vultr
{
    namespace VFSSerialize
    {
        struct VAssetHeader
        {
            const u32 magic_number = 0;
            const u32 version = 0;

            u32 size = 0;
        };
    } // namespace VFSSerialize

    VirtualFilesystem::VirtualFilesystem(const VultrAssetPackage *asset_package, const VultrAssetInfo *asset_info)
    {
        this->asset_package = *asset_package;
        this->asset_info = *asset_info;
        // If one or more of the asset files does not exist, then we need to create them
        if (!fexists(asset_info) || !fexists(asset_package))
        {
            VFSSerialize::VAssetHeader info_header = {VirtualFilesystem::magic_asset_info_number, VirtualFilesystem::version};
            info_header.size = sizeof(info_header);

            FILE *aif = fopen(asset_info->path, "w+");
            fwrite(&info_header, sizeof(info_header), 1, aif);
            fclose(aif);

            VFSSerialize::VAssetHeader package_header = {VirtualFilesystem::magic_asset_info_number, VirtualFilesystem::version};
            package_header.size = sizeof(package_header);

            FILE *apf = fopen(asset_package->path, "w+");
            fwrite(&package_header, sizeof(package_header), 1, aif);
            fclose(aif);
        }
        else
        {
        }
    }

    VirtualFilesystem::~VirtualFilesystem()
    {
    }
} // namespace Vultr

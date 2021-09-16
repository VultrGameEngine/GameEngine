#include <filesystem/virtual_filesystem.h>

namespace Vultr
{
    namespace VFSSerialize
    {
        struct VAssetHeader
        {
            u32 magic_number = VirtualFilesystem::magic_number;
            u32 version = 0;

            u32 size = sizeof(VAssetHeader);
            u32 file_table_offset = size;
        };
    } // namespace VFSSerialize

    static void vfs_write_header(VFSSerialize::VAssetHeader *header, const VultrAssetPackage *package, FILE *f)
    {
        header->size = sizeof(*header);

        fprintf(stdout, "Writing asset package with magic number %u\n", header->magic_number);
        fwrite(&package, header->size, 1, f);
    }

    static bool vfs_read_header(VFSSerialize::VAssetHeader *header, const VultrAssetPackage *package, FILE *f)
    {
        struct AHeader
        {
            u32 magic_number;
            u32 version;
        };
        size_t buf_size = sizeof(AHeader);
        AHeader buff;
        size_t read = fread(&buff, buf_size, 1, f);
        if (read < 1)
        {
            fprintf(stderr, "Failed to read empty asset package!\n");
            return false;
        }

        if (buff.magic_number != VirtualFilesystem::magic_number)
        {
            fprintf(stderr, "Failed to read asset package! File is not a Vultr Asset Package according to the magic number %u in the first 4 bytes of the header. Actual magic number is %u.\n", buff.magic_number,
                    VirtualFilesystem::magic_number);
            return false;
        }

        header->version = buff.version;
        switch (header->version)
        {
            case 0:
            {
                size_t read = fread(header, sizeof(*header), 1, f);
                if (read < 1)
                {
                    fprintf(stderr, "Failed to read asset package! Something went wrong...\n");
                    return false;
                }
                break;
            }
            default:
            {
                fprintf(stderr, "Failed to read asset package! Version %d is not found!\n", header->version);
                return false;
            }
        }

        return true;
    }

    VirtualFilesystem::VirtualFilesystem(const VultrAssetPackage *asset_package)
    {
        this->asset_package = *asset_package;

        // If one or more of the asset files does not exist, then we need to create them
        if (!fexists(asset_package))
        {
            VFSSerialize::VAssetHeader header = {VirtualFilesystem::magic_number, VirtualFilesystem::version};
            FILE *f = fopen(asset_package->path, "wb+");
            vfs_write_header(&header, asset_package, f);
            fclose(f);
            this->fp = fopen(this->asset_package.path, "rb+");
            version = header.version;
        }
        else
        {
            VFSSerialize::VAssetHeader header;
            this->fp = fopen(this->asset_package.path, "rb+");
            bool res = vfs_read_header(&header, asset_package, this->fp);
            assert(res && "Failed to read asset package!");
            version = header.version;
        }
    }

    VirtualFilesystem::~VirtualFilesystem()
    {
    }
} // namespace Vultr

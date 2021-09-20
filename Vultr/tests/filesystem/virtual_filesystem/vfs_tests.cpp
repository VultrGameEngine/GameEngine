#include <gtest/gtest.h>
#define private public
#define protected public

#include <filesystem/virtual_filesystem.h>

using namespace Vultr;

TEST(VirtualFilesystem, Init)
{
    auto resource_dir = Directory("src/");
    ASSERT_TRUE(direxists(&resource_dir));
    auto vfs = VirtualFilesystem(&resource_dir);

    // auto asset_package = VultrAssetPackage("assets.vasset");
    // auto vfs = VirtualFilesystem(&asset_package, true);
    // printf("Virtual file system version %d...\n", vfs.version);
    // auto file = VFILE("models/penis.fbx", 10);
    // vfcreate(&vfs, &file);
}

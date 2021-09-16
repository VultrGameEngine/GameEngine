#include <gtest/gtest.h>
#define private public
#define protected public

#include <filesystem/virtual_filesystem.h>

using namespace Vultr;

TEST(VirtualFilesystem, Init)
{
    auto asset_package = VultrAssetPackage("assets.vasset");
    auto vfs = VirtualFilesystem(&asset_package);
    printf("Virtual file system version %d...\n", vfs.version);
}

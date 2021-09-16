#include <gtest/gtest.h>
#define private public
#define protected public

#include <filesystem/virtual_filesystem.h>

using namespace Vultr;

TEST(VirtualFilesystem, Init)
{
    auto asset_package = VultrAssetPackage("assets.vasset");
    auto asset_info = VultrAssetInfo("assets.vinfo");
    auto vfs = VirtualFilesystem(&asset_package, &asset_info);
}

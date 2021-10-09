#include <gtest/gtest.h>
#define private public
#define protected public

#include <filesystem/resource_manager.h>

using namespace Vultr;

TEST(VirtualFilesystem, Init)
{
    InternalResourceManager<Texture, Mesh> resource_manager;
}

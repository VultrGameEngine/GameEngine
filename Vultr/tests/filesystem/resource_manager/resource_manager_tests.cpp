#include <gtest/gtest.h>
#define private public
#define protected public

#include <engine.hpp>
#include <filesystem/virtual_filesystem.h>

using namespace Vultr;

TEST(ResourceManager, Init)
{
    auto *e = new Engine();
    e->resource_manager = new ResourceManager();

    auto resource = Resource<Texture>(FILE_ASSET("hello.png"), e);

    // resource_manager.incr<Texture>(10);
    // ASSERT_EQ();
}

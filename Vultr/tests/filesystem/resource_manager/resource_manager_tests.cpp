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

    engine_init_resource_threads(e);

    auto resource_directory = Directory("./res/");

    ASSERT_TRUE(dirmake(&resource_directory));
    ASSERT_TRUE(direxists(&resource_directory));

    engine_init_vfs(e, &resource_directory);

    auto hello_png = TextureSource(&resource_directory, "hello.png");
    FILE *f = fopen(hello_png.path, "w+");
    fclose(f);

    auto mesh_obj = ModelSource(&resource_directory, "mesh.obj");
    f = fopen(mesh_obj.path, "w+");
    fclose(f);

    auto shader_glsl = ModelSource(&resource_directory, "shader.glsl");
    f = fopen(shader_glsl.path, "w+");
    fclose(f);

    auto mesh2_obj = ModelSource(&resource_directory, "mesh2.obj");
    f = fopen(mesh2_obj.path, "w+");
    fclose(f);

    auto texture = RESOURCE(Texture, "hello.png", e);
    auto m1 = RESOURCE(Mesh, "mesh.obj", e);

    auto shader = RESOURCE(Shader, "shader.glsl", e);

    usleep(1000);

    auto dup_m = RESOURCE(Mesh, "mesh.obj", e);
    auto m2 = RESOURCE(Mesh, "mesh2.obj", e);

    // USleeps are necessary to make sure that we don't remove the files too early
    usleep(1000);

    ASSERT_TRUE(dirremove(&resource_directory));
    ASSERT_FALSE(direxists(&resource_directory));
}

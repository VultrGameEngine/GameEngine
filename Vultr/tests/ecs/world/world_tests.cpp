#include <gtest/gtest.h>
#define private public
#define protected public

#include <vultr.hpp>

using namespace Vultr;
TEST(World, Serialization)
{
    Engine &vultr = engine_global();

    float lastTime = 0;
    engine_init(vultr, true);
    engine_load_game(vultr, "/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");
    engine_init_game(vultr);
    File output_file = File("/home/brandon/Dev/GameEngine/test_world.json");
    save_world(get_current_world(), output_file);
}

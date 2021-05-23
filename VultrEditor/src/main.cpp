#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>

using namespace Vultr;
int main(void)
{
    Engine &vultr = engine_global();

    float lastTime = 0;
    Path::set_resource_path("/home/brandon/Dev/GameEngine/SandboxTesting/res/");
    engine_init(vultr, true);
    engine_load_game(vultr, "/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");
    engine_global().game->RegisterComponents();

    while (!vultr.should_close)
    {
        engine_update_game(vultr, lastTime, Editor::Get()->playing);
        Editor::Get()->Render();
        glfwSwapBuffers(vultr.window);
        glfwPollEvents();
    }
}

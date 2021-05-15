#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>

using namespace Vultr;
int main(void)
{
    Engine &vultr = engine_global();

    float lastTime = 0;
    engine_init(vultr, true);
    engine_load_game(vultr, "/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");

    while (!vultr.should_close)
    {
        engine_update_game(vultr, lastTime);
        Vultr::Editor::Editor::Get()->Render();
        glfwSwapBuffers(vultr.window);
        glfwPollEvents();
    }
    // vultr.Destroy();
}

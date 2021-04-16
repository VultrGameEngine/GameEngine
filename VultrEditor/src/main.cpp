#include <Vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>

using namespace Vultr;
int main(void)
{
    Engine &vultr = Engine::Get();

    float lastTime = 0;
    vultr.Init(true);
    vultr.LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/Debug/"
                   "libSandboxTesting.so");

    while (!vultr.should_close)
    {
        vultr.UpdateGame(lastTime);
        Vultr::Editor::Editor::Get()->Render();
        glfwSwapBuffers(vultr.window);
        glfwPollEvents();
    }
    vultr.Destroy();
}

#include <Vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>

using namespace Vultr;
int main(void)
{
    Engine *vultr = new Engine();

    float lastTime = 0;
    vultr->Init(true);
    vultr->LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");

    while (!vultr->should_close)
    {
        vultr->UpdateGame(lastTime);
        Vultr::Editor::Editor::Get()->Render();
        glfwSwapBuffers(vultr->window);
    }
    vultr->Destroy();

    // imgui_implopengl3_shutdown();
    // imgui_implglfw_shutdown();

    // ImGui::DestroyContext();
}

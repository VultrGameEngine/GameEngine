#include <Vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>

using namespace Brick3D;
int main(void)
{
    Vultr *vultr = new Vultr();

    float lastTime = 0;
    vultr->Init(true);
    vultr->LoadGame("/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");

    while (true)
    {
        vultr->UpdateGame(lastTime);
        Brick3D::Editor::Editor::Get()->Render();
        glfwSwapBuffers(vultr->window);
    }

    // imgui_implopengl3_shutdown();
    // imgui_implglfw_shutdown();

    // ImGui::DestroyContext();
}

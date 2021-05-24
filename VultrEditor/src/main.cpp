#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>

using namespace Vultr;
int main(void)
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

    float lastTime = 0;
#ifdef _WIN32
    Path::set_resource_path("C:/Users/Brand/Dev/GameEngine/SandboxTesting/res/");
#else
    Path::set_resource_path("/home/brandon/Dev/GameEngine/SandboxTesting/res/");
#endif
    engine_init(vultr, true);
#ifdef _WIN32
    engine_load_game(vultr, "C:/Users/Brand/Dev/GameEngine/SandboxTesting/build/Debug/Game.dll");
#else
    engine_load_game(vultr, "/home/brandon/Dev/GameEngine/SandboxTesting/build/libGame.so");
#endif
    engine_global()->game->RegisterComponents();
    vultr->game->SetImGuiContext(ImGui::GetCurrentContext());

    while (!vultr->should_close)
    {
        engine_update_game(vultr, lastTime, Editor::Get()->playing);
        Editor::Get()->Render();
        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}

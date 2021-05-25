#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>
#include <helpers/directory.h>

using namespace Vultr;
int main(void)
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

    Directory cwd = Directory(std::filesystem::current_path().string());
    auto files = cwd.GetFiles();

    File vultr_helper = cwd / File("VultrHelper");

    if (files.find(vultr_helper) == files.end())
    {
        std::cout << "No VultrHelper found in current working directory!" << std::endl;
        return 1;
    }
    auto sub_directories = cwd.GetDirectories();

    Directory resource_directory = cwd / Directory("res");

    if (sub_directories.find(resource_directory) == sub_directories.end())
    {
        std::cout << "No resource directory found in current working directory!" << std::endl;
        return 1;
    }

    Directory build_directory = cwd / Directory("build");

    if (sub_directories.find(build_directory) == sub_directories.end())
    {
        std::cout << "No build directory found in current working directory!" << std::endl;
        return 1;
    }

    File dll = build_directory / File("libGame.so");

    auto build_files = build_directory.GetFiles();

    if (build_files.find(dll) == build_files.end())
    {
        std::cout << "No DLL found in build directory" << std::endl;
        return 1;
    }

    float lastTime = 0;

#ifdef _WIN32
    Path::set_resource_path("C:/Users/Brand/Dev/GameEngine/SandboxTesting/res/");
#else
    Path::set_resource_path(resource_directory.GetPath().string());
#endif

    engine_init(vultr, true);

#ifdef _WIN32
    engine_load_game(vultr, "C:/Users/Brand/Dev/GameEngine/SandboxTesting/build/Debug/Game.dll");
#else
    engine_load_game(vultr, dll.GetPath().string().c_str());
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

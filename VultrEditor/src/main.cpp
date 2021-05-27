#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>
#include <helpers/directory.h>
#include <fonts/fork_awesome.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>
#include <helpers/texture_importer.h>

INCBIN(fork_awesome, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/forkawesome-webfont.ttf");

using namespace Vultr;
int main(void)
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

#ifndef _WIN32
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
#endif
    float lastTime = 0;

#ifdef _WIN32
    Path::set_resource_path("C:/Users/Brand/Dev/Monopoly/res/");
#else
    Path::set_resource_path(resource_directory.GetPath().string());
#endif

    engine_init(vultr, true);

#ifdef _WIN32
    engine_load_game(vultr, "C:/Users/Brand/Dev/Monopoly/build/Debug/Game.dll");
#else
    engine_load_game(vultr, dll.GetPath().string().c_str());
#endif

    engine_global()->game->RegisterComponents();
    vultr->game->SetImGuiContext(ImGui::GetCurrentContext());
    engine_global()->on_edit = Editor::OnEdit;

    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    ImGuiIO &io = ImGui::GetIO();
    // Editor::Get()->icon_large = io.Fonts->AddFontFromMemoryTTF((void *)gfork_awesome_data, gfork_awesome_size, 60.0f, &icons_config, icons_ranges);
    Editor::Get()->icon_small = io.Fonts->AddFontFromMemoryTTF((void *)gfork_awesome_data, gfork_awesome_size, 24.0f, &icons_config, icons_ranges);

    Editor::Get()->current_directory = resource_directory;

    while (!vultr->should_close)
    {
        engine_update_game(vultr, lastTime, Editor::Get()->playing);
        Editor::Get()->Render();
        glfwSwapBuffers(vultr->window);
        glfwPollEvents();
    }
}

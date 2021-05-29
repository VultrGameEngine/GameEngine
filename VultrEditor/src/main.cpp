#include <vultr.hpp>
#include <editor/editor.hpp>
#include <iostream>
#include <helpers/path.h>
#include <helpers/directory.h>
#include <fonts/fork_awesome.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>
#include <helpers/texture_importer.h>
#include <set>

#ifndef SOURCE_PATH
#define SOURCE_PATH "INVALID SOURCE"
#endif

INCBIN(fork_awesome, SOURCE_PATH "res/forkawesome-webfont.ttf");
INCBIN(roboto, SOURCE_PATH "res/roboto.ttf");

using namespace Vultr;
int main(void)
{
    engine_global() = new Engine();
    auto *vultr = engine_global();

#ifndef _WIN32
    Directory cwd = Directory(std::filesystem::current_path().string());
    auto _f = directory_get_files(cwd);
    auto files = std::set(_f.begin(), _f.end());

    File vultr_helper = cwd / File("VultrHelper");

    if (files.find(vultr_helper) == files.end())
    {
        std::cout << "No VultrHelper found in current working directory!" << std::endl;
        return 1;
    }
    auto _sd = directory_get_sub_directories(cwd);
    auto sub_directories = std::set(_sd.begin(), _sd.end());

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

    auto _bf = directory_get_files(build_directory);
    auto build_files = std::set(_bf.begin(), _bf.end());

    if (build_files.find(dll) == build_files.end())
    {
        std::cout << "No DLL found in build directory" << std::endl;
        return 1;
    }
#endif
    float lastTime = 0;

#ifdef _WIN32
    Path::set_resource_path(resource_directory.path);
#else
    change_working_directory(resource_directory.path);
#endif

    engine_init(vultr, true);

#ifdef _WIN32
    engine_load_game(vultr, dll.path.string().c_str());
#else
    engine_load_game(vultr, dll.path.string().c_str());
#endif

    engine_global()->game->RegisterComponents();
    vultr->game->SetImGuiContext(ImGui::GetCurrentContext());
    engine_global()->on_edit = Editor::OnEdit;

    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF((void *)groboto_data, groboto_size, 30);
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

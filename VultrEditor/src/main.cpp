#include <vultr.hpp>
#include <editor.h>
#include <filesystem/directory.h>
#include <fonts/fork_awesome.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>
#include <windows/asset_browser.h>
#include <windows/component_window.h>
#include <windows/entity_window.h>
#include <windows/game_window.h>
#include <windows/scene_window.h>
#ifndef SOURCE_PATH
#define SOURCE_PATH "INVALID SOURCE"
#endif

#ifndef _WIN32
INCBIN(fork_awesome, SOURCE_PATH "res/forkawesome-webfont.ttf");
INCBIN(roboto, SOURCE_PATH "res/roboto.ttf");
#else
INCBIN(fork_awesome, "../res/forkawesome-webfont.ttf");
INCBIN(roboto, "../res/roboto.ttf");
#endif

using namespace Vultr;
int main(void)
{
    auto *e = new Engine();

    Directory cwd;
    dircurrentworking(&cwd);
#ifndef _WIN32
    GenericFile vultr_helper = GenericFile(&cwd, "VultrHelper");
#else
    GenericFile vultr_helper = GenericFile(&cwd, "VultrHelper.exe");
#endif

    if (!fexists(&vultr_helper))
    {
        fprintf(stderr, "No VultrHelper found in current working directory %s", cwd.path);
        return 1;
    }

    Directory resource_directory = Directory(&cwd, "res");

    if (!direxists(&resource_directory))
    {
        fprintf(stderr, "No resource directory found in current working directory %s", cwd.path);
        return 1;
    }

#ifndef _WIN32
    Directory build_directory = Directory(&cwd, "build");
#else
    Directory build_directory = Directory(&cwd, "out/build/x64-Debug");
#endif

    if (!direxists(&build_directory))
    {
        fprintf(stderr, "No build directory found in current working directory %s", cwd.path);
        return 1;
    }

#ifndef _WIN32
    DLLSource dll = DLLSource(&build_directory, "libGame.so");
#else
    DLLSource dll = DLLSource(&build_directory, "Game.dll");
#endif

    if (!fexists(&dll))
    {
        fprintf(stderr, "No DLL found in build directory %s", cwd.path);
        return 1;
    }
    float lastTime = 0;

    dirchangeworking(&resource_directory);

    engine_init(e, true);

    engine_load_game(e, &dll);

    auto *reload_watcher = init_reload_watcher(&dll);

    auto *editor = new Editor();
    editor->reload_watcher = reload_watcher;
    add_editor(e, static_cast<void *>(editor));

    e->game->set_imgui_context(ImGui::GetCurrentContext());
    e->on_edit = on_edit;

    static const ImWchar icons_ranges[] = {ICON_MIN_FK, ICON_MAX_FK, 0};
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromMemoryTTF((void *)groboto_data, groboto_size, 30);
    io.Fonts->AddFontFromMemoryTTF((void *)gfork_awesome_data, gfork_awesome_size, 24.0f, &icons_config, icons_ranges);

    register_asset_browser(e, editor, resource_directory);
    register_component_window(e, editor);
    register_entity_window(e, editor);
    register_game_window(e, editor);
    register_scene_window(e, editor);

    while (!e->should_close)
    {
        auto tick = engine_update_game(e, lastTime, editor->game_manager.playing);
        editor_render(e, editor, tick);
        glfwSwapBuffers(e->window);
        glfwPollEvents();
    }
}

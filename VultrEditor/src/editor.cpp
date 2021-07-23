#include <vultr.hpp>
#include <windows/component_window.h>
#include <windows/game_window.h>
#include <windows/scene_window.h>
#include <windows/entity_window.h>
#include <windows/asset_browser.h>
#include <editor.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/world/internal_world.hpp>
#include <core/models/event.h>

using namespace Vultr;
Editor::Editor() : selected_entity(Entity(0))
{
}

void editor_register_window(Vultr::Engine *e, Editor *editor, WindowRenderer renderer, void *state)
{
    editor->windows.push_back({
        .renderer = renderer,
        .state = state,
    });
}

void delete_entity(Vultr::Engine *e, Editor *editor)
{
    if (editor->selected_entity == INVALID_ENTITY)
        return;
    auto *world = get_current_world(e);

    auto &component_manager = world_get_component_manager(world);
    auto *event = new EntityDestroyEvent();
    Entity selected_entity = editor->selected_entity;
    for (auto [type, array] : component_manager.component_arrays)
    {
        void *component = array->InternalGetData(selected_entity);
        if (component != nullptr)
        {
            event->components[type] = component;
        }
    }
    event->entity = selected_entity;
    destroy_entity(e, selected_entity);
    on_edit(editor, event);
}

void select_entity(Vultr::Engine *e, Editor *editor, Entity entity)
{
    editor->selected_entity = entity;
}

void editor_render(Vultr::Engine *e, Editor *editor, const UpdateTick &tick)
{
    glDisable(GL_DEPTH_TEST);

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
#define TOOLBAR_HEIGHT 100
    ImGui::NewFrame();
    {
        ImGuizmo::BeginFrame();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + TOOLBAR_HEIGHT));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - TOOLBAR_HEIGHT));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar(3);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        editor->dockspace = ImGui::GetID("HUB_DockSpace");
        ImGui::DockSpace(editor->dockspace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);

        for (auto &pair : editor->windows)
        {
            ImGui::SetNextWindowDockID(editor->dockspace, ImGuiCond_FirstUseEver);
            pair.renderer(e, editor, tick, pair.state);
        }
        ImGui::End();
    }
    {
        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, TOOLBAR_HEIGHT));
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags =
            0 | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("TOOLBAR", NULL, window_flags);
        ImGui::PopStyleVar();

        auto &gm = editor->game_manager;

        if (gm.playing)
        {
            if (ImGui::Button("Pause"))
            {
                gm.playing = false;
            }
        }
        else
        {
            if (ImGui::Button("Play"))
            {
                if (!gm.game_running)
                {
                    World *cached_world = new InternalWorld();
                    auto *world = get_current_world(e);
                    component_manager_copy(cached_world->component_manager, world->component_manager);
                    cached_world->entity_manager = world->entity_manager;
                    // cached_world->system_manager = SystemManager();
                    cached_world->system_manager = world->system_manager;
                    gm.cached_world = cached_world;
                    bool empty = world_get_system_manager(world).system_providers.empty();

                    engine_init_game(e);
                    gm.game_running = true;
                }

                gm.playing = true;
            }
        }

        if (gm.game_running)
        {
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
            {
                gm.playing = false;
                gm.game_running = false;
                engine_detach_game(e);
                change_world(e, gm.cached_world);
                gm.cached_world = nullptr;
            }
        }

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
    glEnable(GL_DEPTH_TEST);

    // Once we are done rendering everything, THEN we will look at reloading the DLL
    // Only do this when there isn't a game actively running
    if (!editor->game_manager.game_running)
    {
        bool reload = watch_dll(editor->reload_watcher);
        if (reload)
        {
        }
    }
}

void save(Vultr::Engine *e, Editor *editor)
{
    save_world(e, get_current_world(e), VultrSource("test_world.vultr"));
}

void duplicate_entity(Vultr::Engine *e, Editor *editor)
{
    Entity selected_entity = editor->selected_entity;
    if (selected_entity == INVALID_ENTITY)
        return;
    auto *world = get_current_world(e);
    Entity duplicate = create_entity(world);
    auto &component_manager = world_get_component_manager(world);
    for (auto [type, data] : e->component_registry.components)
    {
        auto *component_array = component_manager.component_arrays[type];
        if (component_array->HasData(selected_entity))
        {
            component_array->DuplicateData(selected_entity, duplicate);
        }
    }
    auto signature = get_entity_signature(world, selected_entity);
    system_manager_entity_signature_changed(e, world_get_system_manager(world), duplicate, signature);
    system_manager_entity_signature_changed(e, e->system_manager, duplicate, signature);
    entity_manager_set_signature(world_get_entity_manager(world), duplicate, signature);
}

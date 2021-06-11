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

Editor &get_editor()
{
    static Editor instance;
    return instance;
}

void editor_register_window(WindowRenderer renderer, void *state)
{
    get_editor().windows.push_back({
        .renderer = renderer,
        .state = state,
    });
}

void delete_entity()
{
    auto &m = get_editor();
    if (m.selected_entity == INVALID_ENTITY)
        return;
    auto *world = get_current_world();

    auto &component_manager = world_get_component_manager(world);
    auto *event = new EntityDestroyEvent();
    Entity selected_entity = m.selected_entity;
    for (auto [type, array] : component_manager.component_arrays)
    {
        void *component = array->InternalGetData(selected_entity);
        if (component != nullptr)
        {
            event->components[type] = component;
        }
    }
    event->entity = selected_entity;
    destroy_entity(selected_entity);
    on_edit(event);
}

void select_entity(Entity entity)
{
    get_editor().selected_entity = entity;
}

void editor_render(const UpdateTick &tick)
{
    auto &m = get_editor();

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
        m.dockspace = ImGui::GetID("HUB_DockSpace");
        ImGui::DockSpace(m.dockspace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);

        for (auto &pair : m.windows)
        {
            ImGui::SetNextWindowDockID(m.dockspace, ImGuiCond_FirstUseEver);
            pair.renderer(tick, pair.state);
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

        auto &gm = m.game_manager;

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
                    auto *world = get_current_world();
                    component_manager_copy(cached_world->component_manager, world->component_manager);
                    cached_world->entity_manager = world->entity_manager;
                    cached_world->system_manager = world->system_manager;
                    if (gm.cached_world != nullptr)
                        delete gm.cached_world;
                    gm.cached_world = cached_world;

                    engine_init_game(engine_global());
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
                change_world(gm.cached_world);
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
}

void save()
{
    save_world(get_current_world(), VultrSource("test_world.vultr"));
}

void duplicate_entity()
{
    auto &m = get_editor();
    Entity selected_entity = m.selected_entity;
    if (selected_entity == INVALID_ENTITY)
        return;
    auto *world = get_current_world();
    Entity duplicate = create_entity(world);
    auto &component_manager = world_get_component_manager(world);
    for (auto [type, data] : engine_global()->component_registry.components)
    {
        auto *component_array = component_manager.component_arrays[type];
        if (component_array->HasData(selected_entity))
        {
            component_array->DuplicateData(selected_entity, duplicate);
        }
    }
    auto signature = get_entity_signature(world, selected_entity);
    system_manager_entity_signature_changed(world_get_system_manager(world), duplicate, signature);
    system_manager_entity_signature_changed(engine_global()->system_manager, duplicate, signature);
    entity_manager_set_signature(world_get_entity_manager(world), duplicate, signature);
}

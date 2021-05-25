#include <vultr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/systems/render_system.h>
#include <editor/core/windows/component_window.hpp>
#include <editor/core/windows/game_window.hpp>
#include <editor/core/windows/scene_window.hpp>
#include <editor/core/windows/entity_window.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/world/internal_world.hpp>

using namespace Vultr;
Editor::Editor() : selected_entity(Entity(0))
{
    windows.push_back(new GameWindow());
    windows.push_back(new SceneWindow());
    windows.push_back(new ComponentWindow());
    windows.push_back(new EntityWindow());
}

Editor::~Editor()
{
    windows.clear();
    windows.shrink_to_fit();
}

void Editor::Render()
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
        Get()->dockspace = ImGui::GetID("HUB_DockSpace");
        ImGui::DockSpace(Get()->dockspace, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoResize);

        for (Window *window : Get()->windows)
        {
            ImGui::SetNextWindowDockID(Get()->dockspace, ImGuiCond_FirstUseEver);
            window->Render();
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

        if (Editor::Get()->playing)
        {
            if (ImGui::Button("Pause"))
            {
                Editor::Get()->playing = false;
            }
        }
        else
        {
            if (ImGui::Button("Play"))
            {
                if (!Editor::Get()->game_running)
                {
                    World *cached_world = new InternalWorld();
                    auto *world = get_current_world();
                    component_manager_copy(cached_world->component_manager, world->component_manager);
                    cached_world->entity_manager = world->entity_manager;
                    cached_world->system_manager = world->system_manager;
                    if (Editor::Get()->cached_world != nullptr)
                        delete Editor::Get()->cached_world;
                    Editor::Get()->cached_world = cached_world;

                    engine_init_game(engine_global());
                    Editor::Get()->game_running = true;
                }

                Editor::Get()->playing = true;
            }
        }

        if (Editor::Get()->game_running)
        {
            ImGui::SameLine();
            if (ImGui::Button("Stop"))
            {
                Editor::Get()->playing = false;
                Editor::Get()->game_running = false;
                change_world(Editor::Get()->cached_world);
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

void Editor::Save()
{
    save_world(get_current_world(), File("test_world.json"));
}

void Editor::DuplicateEntity()
{
    Entity selected_entity = Get()->selected_entity;
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

void Editor::ClearSelections()
{
    selected_entity = selected_entity_queue.back();
    while (!selected_entity_queue.empty())
    {
        selected_entity_queue.pop();
    }
}
void Editor::QueueEntitySelection(Vultr::Entity entity)
{
    selected_entity_queue.push(entity);
}

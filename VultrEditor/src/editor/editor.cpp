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

        ImGui::Button("Toolbar goes here", ImVec2(0, 37));

        if (ImGui::Button("Play"))
        {
            engine_init_game(engine_global());
            Editor::Get()->playing = true;
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

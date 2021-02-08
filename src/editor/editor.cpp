#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <core/systems/render_system.h>
#include <editor/core/windows/game_window.hpp>
#include <editor/core/windows/scene_window.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

namespace Brick3D::Editor
{
Editor::Editor()
{
    windows.push_back(new GameWindow());
    windows.push_back(new SceneWindow());
}

Editor::~Editor()
{
    windows.clear();
    windows.shrink_to_fit();
}

void Editor::Editor::Render()
{
    glDisable(GL_DEPTH_TEST);

    ImGui_ImplOpenGL3_NewFrame();

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    static bool dockspaceOpen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar(3);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    Get()->dockspace = ImGui::GetID("HUB_DockSpace");
    ImGui::DockSpace(Get()->dockspace, ImVec2(0.0f, 0.0f),
                     ImGuiDockNodeFlags_None |
                         ImGuiDockNodeFlags_PassthruCentralNode |
                         ImGuiDockNodeFlags_NoResize);

    for (Window *window : Get()->windows)
    {
        ImGui::SetNextWindowDockID(Get()->dockspace, ImGuiCond_FirstUseEver);
        window->Render();
    }
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    GLFWwindow *backup_current_context = glfwGetCurrentContext();
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
    glfwMakeContextCurrent(backup_current_context);
    glEnable(GL_DEPTH_TEST);
}
} // namespace Brick3D::Editor

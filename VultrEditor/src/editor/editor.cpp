#include <vultr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <core/systems/render_system.h>
#include <editor/core/windows/component_window.hpp>
#include <editor/core/windows/game_window.hpp>
#include <editor/core/windows/scene_window.hpp>
#include <editor/core/windows/entity_window.hpp>
#include <editor/core/windows/asset_browser.hpp>
#include <editor/editor.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/world/internal_world.hpp>
#include <stb_image/stb_image.h>
#define INCBIN_STYLE INCBIN_STYLE_SNAKE
#include <incbin/incbin.h>

INCBIN(folder_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/folder.png");
INCBIN(file_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/document.png");
INCBIN(c_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/c.png");
INCBIN(image_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/image.png");
INCBIN(shader_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/s.png");
INCBIN(model_icon, "/home/brandon/Dev/Monopoly/GameEngine/VultrEditor/res/3D.png");

using namespace Vultr;
Editor::Editor() : selected_entity(Entity(0)), current_directory(Path::get_resource_path())
{
    windows.push_back(new GameWindow());
    windows.push_back(new SceneWindow());
    windows.push_back(new ComponentWindow());
    windows.push_back(new EntityWindow());
    windows.push_back(new AssetBrowser());

    folder_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gfolder_icon_data, gfolder_icon_size, *folder_icon);

    file_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gfile_icon_data, gfile_icon_size, *file_icon);

    c_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gc_icon_data, gc_icon_size, *c_icon);

    image_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gimage_icon_data, gimage_icon_size, *image_icon);

    shader_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gshader_icon_data, gshader_icon_size, *shader_icon);

    model_icon = new Texture(GL_TEXTURE_2D);
    TextureImporter::Import(gmodel_icon_data, gmodel_icon_size, *model_icon);
}

Editor::~Editor()
{
    windows.clear();
    windows.shrink_to_fit();
}

void Editor::OnEdit(EditEvent *e)
{
    auto *editor = Editor::Get();
    s32 stack_size = editor->event_stack.size();
    if (editor->event_index < stack_size - 1)
    {
        for (int i = editor->event_index + 1; i < editor->event_stack.size(); i++)
        {
            delete editor->event_stack[i];
        }
        editor->event_stack.erase(editor->event_stack.begin() + editor->event_index + 1, editor->event_stack.end());
    }
    editor->event_index++;
    Editor::Get()->event_stack.push_back(e);
}
void Editor::Undo()
{
    auto *editor = Editor::Get();
    if (editor->event_stack.size() == 0)
        return;
    if (editor->event_index <= -1)
        return;
    auto *event = editor->event_stack[editor->event_index];
    event->Undo(engine_global());
    editor->event_index--;
}

void Editor::Redo()
{
    auto *editor = Editor::Get();
    if (editor->event_stack.size() == 0)
        return;
    s32 stack_size = editor->event_stack.size();
    if (editor->event_index + 1 >= stack_size)
        return;
    editor->event_index++;
    auto *event = editor->event_stack[editor->event_index];
    event->Redo(engine_global());
}

void Editor::DeleteEntity()
{
    auto *editor = Editor::Get();
    if (editor->selected_entity == INVALID_ENTITY)
        return;
    auto *world = get_current_world();

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
    destroy_entity(selected_entity);
    OnEdit(event);
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

#pragma once
#include "imgui/window.hpp"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/entity/entity.hpp>
#include <vector>
#include <queue>
#include <rendering/models/texture.h>
#include <ecs/world/world.hpp>
#include <core/models/event.h>
#include <helpers/directory.h>

class Editor
{
  public:
    Editor();
    ~Editor();
    static Editor *Get()
    {
        static Editor *instance;
        if (instance == nullptr)
        {
            instance = new Editor();
        }
        return instance;
    }

    static void OnEdit(Vultr::EditEvent *e);
    static void Undo();
    static void Redo();

    static void DeleteEntity();

    static unsigned int GetDockSpace();
    static void Render();
    static void Save();
    static void DuplicateEntity();
    Vultr::Entity selected_entity;
    ImGuizmo::OPERATION current_operation = ImGuizmo::OPERATION::TRANSLATE;

    void QueueEntitySelection(Vultr::Entity entity);
    void ClearSelections();

    bool playing = false;
    bool game_running = false;
    Vultr::World *cached_world = nullptr;

    Vultr::Directory current_directory;

    ImFont *icon_small;
    ImFont *icon_large;

    Vultr::Texture *folder_icon;
    Vultr::Texture *file_icon;
    Vultr::Texture *c_icon;
    Vultr::Texture *image_icon;
    Vultr::Texture *shader_icon;
    Vultr::Texture *model_icon;
    Vultr::Texture *vultr_icon;

  private:
    std::vector<Window *> windows;
    std::queue<Vultr::Entity> selected_entity_queue;
    std::vector<Vultr::EditEvent *> event_stack;
    s32 event_index = -1;
    unsigned int dockspace = 1;
};

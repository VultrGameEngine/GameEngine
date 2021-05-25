#pragma once
#include "imgui/window.hpp"
#include <imgui/imgui.h>
#include <ImGuizmo/ImGuizmo.h>
#include <ecs/entity/entity.hpp>
#include <vector>
#include <queue>
#include <rendering/models/texture.h>
#include <ecs/world/world.hpp>

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

  private:
    std::vector<Window *> windows;
    std::queue<Vultr::Entity> selected_entity_queue;
    unsigned int dockspace = 1;
};

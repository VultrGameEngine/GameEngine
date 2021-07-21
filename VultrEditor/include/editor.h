#pragma once
#include <vultr.hpp>
#include <vector>
#include "editor_texture_manager.h"
#include "edit_event_manager.h"
#include "game_manager.h"

typedef void (*WindowRenderer)(Vultr::Engine *e, const Vultr::UpdateTick &tick, void *state);

struct Window
{
    WindowRenderer renderer;
    void *state;
};

struct Editor
{
    Editor();
    // For ImGui
    u32 dockspace = 1;

    // Holds a list of render callbacks that will be triggered each frame for all of the windows
    std::vector<Window> windows;

    // Holds the globally selected entity
    Vultr::Entity selected_entity;

    // State info for the running game instance
    GameManager game_manager;

    // All edit events are fed through here
    EditEventManager edit_event_manager;

    // Textures used throughout the program
    EditorTextureManager texture_manager;
};

// Get singleton instance
Editor &get_editor();

void editor_register_window(Vultr::Engine *e, WindowRenderer renderer, void *state);
void editor_render(Vultr::Engine *e, const Vultr::UpdateTick &tick);
void select_entity(Vultr::Entity entity);

void on_edit(Vultr::EditEvent *e);
void undo(Vultr::Engine *e);
void redo(Vultr::Engine *e);

void save(Vultr::Engine *e);

void duplicate_entity(Vultr::Engine *e);

// Delete the currently selected entity
void delete_entity(Vultr::Engine *e);

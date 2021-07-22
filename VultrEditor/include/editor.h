#pragma once
#include <vultr.hpp>
#include <vector>
#include <services/reload_watcher.h>
#include "editor_texture_manager.h"
#include "edit_event_manager.h"
#include "game_manager.h"

typedef void (*WindowRenderer)(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick, void *state);

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

    // Used to hot relaod the game DLL
    ReloadWatcher *reload_watcher;
};

void editor_register_window(Vultr::Engine *e, Editor *editor, WindowRenderer renderer, void *state);
void editor_render(Vultr::Engine *e, Editor *editor, const Vultr::UpdateTick &tick);
void select_entity(Vultr::Engine *e, Editor *editor, Vultr::Entity entity);

void on_edit(void *editor, Vultr::EditEvent *e);
void undo(Vultr::Engine *e, Editor *editor);
void redo(Vultr::Engine *e, Editor *editor);

void save(Vultr::Engine *e, Editor *editor);

void duplicate_entity(Vultr::Engine *e, Editor *editor);

// Delete the currently selected entity
void delete_entity(Vultr::Engine *e, Editor *editor);

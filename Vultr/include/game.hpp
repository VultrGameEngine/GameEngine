#pragma once
namespace Vultr
{
    struct Engine;
}
#include <core/models/update_tick.h>
#include <imgui/imgui.h>
#include "scene.hpp"
#include <types/types.hpp>
#include <ecs/world/world.hpp>

struct Game
{
    virtual void register_components(Vultr::Engine *e) = 0;
    virtual Scene *init_scene(Vultr::Engine *e, Vultr::World *world) = 0;

    Scene *perform_init_scene(Vultr::Engine *e, Vultr::World *world);

    virtual void flush(Vultr::Engine *e) = 0;

    void perform_flush(Vultr::Engine *e);

    virtual void set_imgui_context(ImGuiContext *context) = 0;

    void update(Vultr::Engine *e, const Vultr::UpdateTick &tick);
    void flush_scene(Vultr::Engine *e);
    virtual ~Game(){};

    Scene *scene = nullptr;
};

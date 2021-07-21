#pragma once
#include <engine.hpp>
#include <types/types.hpp>
#include <core/models/update_tick.h>

namespace Vultr::InputSystem
{
    void register_system(Engine *e);
    void init(Engine *e);
    void update(Engine *e, const UpdateTick &tick);
} // namespace Vultr::InputSystem

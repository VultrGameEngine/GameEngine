#pragma once
#include <types/types.hpp>
#include <core/models/update_tick.h>

namespace Vultr::InputSystem
{
    void register_system();
    void init();
    void update(const UpdateTick &tick);
} // namespace Vultr::InputSystem

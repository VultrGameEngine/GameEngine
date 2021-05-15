#pragma once
#include <ecs/entity/entity.hpp>

namespace Vultr::LightSystem
{
    void register_system();
    void on_create_entity(Entity entity);
} // namespace Vultr::LightSystem

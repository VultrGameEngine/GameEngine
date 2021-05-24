#pragma once
#include <ecs/entity/entity.hpp>

namespace Vultr::LightSystem
{
    void register_system();
    void update();
    void on_destroy_entity(Entity entity);
} // namespace Vultr::LightSystem

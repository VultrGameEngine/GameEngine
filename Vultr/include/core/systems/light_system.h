#pragma once
#include <engine.hpp>
#include <ecs/entity/entity.hpp>

namespace Vultr::LightSystem
{
    void register_system(Engine *e);
    void update(Engine *e);
    void on_destroy_entity(Engine *e, Entity entity);
} // namespace Vultr::LightSystem

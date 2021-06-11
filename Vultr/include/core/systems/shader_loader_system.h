#pragma once
#include <ecs/entity/entity.hpp>

struct MaterialComponent;
namespace Vultr::ShaderLoaderSystem
{
    void register_system();
    void update();
    void load_shader(const MaterialComponent &mat);

    void on_create_entity(Entity entity);
    void on_destroy_entity(Entity entity);
} // namespace Vultr::ShaderLoaderSystem

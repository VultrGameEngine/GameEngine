#pragma once
#include <engine.hpp>
#include <ecs/entity/entity.hpp>

struct MaterialComponent;
namespace Vultr::ShaderLoaderSystem
{
    void register_system(Engine *e);
    void update(Engine *e);
    void load_shader(Engine *e, const MaterialComponent &mat);

    void on_create_entity(Engine *e, Entity entity);
    void on_destroy_entity(Engine *e, Entity entity);
} // namespace Vultr::ShaderLoaderSystem

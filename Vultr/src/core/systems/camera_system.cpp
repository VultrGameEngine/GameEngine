#include <core/components/camera_component.h>
#include <core/systems/camera_system.h>
#include <engine.hpp>

namespace Vultr::CameraSystem
{
    void on_create_entity(Engine *e, Entity entity)
    {
        auto &camera_component = entity_get_component<CameraComponent>(e, entity);
        if (camera_component.enabled)
        {
            get_provider(e).camera = entity;
        }
    }

    void on_destroy_entity(Engine *e, Entity entity)
    {
        auto &provider = get_provider(e);
        if (provider.camera == entity)
        {
            provider.camera = INVALID_ENTITY;
            for (Entity camera_entity : provider.entities)
            {
                auto &camera_component = entity_get_component<CameraComponent>(e, entity);
                if (camera_component.enabled)
                {
                    provider.camera = entity;
                    break;
                }
            }
        }
    }

    void register_system(Engine *e)
    {
        Signature signature;
        signature.set(get_component_type<CameraComponent>(e), true);
        signature.set(get_component_type<TransformComponent>(e), true);
        register_global_system<Component>(e, signature, on_create_entity, on_destroy_entity);
    }
} // namespace Vultr::CameraSystem

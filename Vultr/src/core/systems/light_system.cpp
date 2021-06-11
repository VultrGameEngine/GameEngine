#include <core/components/light_component.h>
#include <core/components/transform_component.h>
#include <core/systems/light_system.h>
#include <core/system_providers/light_system_provider.h>
#include <ecs/world/world.hpp>
#include <engine.hpp>

namespace Vultr::LightSystem
{
    void register_system()
    {
        Signature signature;
        signature.set(get_component_type<LightComponent>(), true);
        signature.set(get_component_type<TransformComponent>(), true);
        register_global_system<Component>(signature, nullptr, on_destroy_entity);
    }

    static void remove_extraneous_point_lights(Entity e)
    {
        auto &provider = get_provider();
        if (provider.point_lights.find(e) != provider.point_lights.end())
        {
            provider.point_lights.erase(e);
        }
    }

    void update()
    {
        auto &provider = get_provider();
        for (auto e : provider.entities)
        {
            auto &light_component = entity_get_component<LightComponent>(e);
            switch (light_component.type)
            {
            case LightComponent::DirectionalLight: {
                if (provider.directional_light != INVALID_ENTITY && provider.directional_light != e)
                {
                    std::cout << "Vultr only supports 1 directional light per scene!";
                }
                else
                {
                    provider.directional_light = e;
                }
                remove_extraneous_point_lights(e);
                break;
            }
            case LightComponent::PointLight: {
                provider.point_lights.insert(e);
                break;
            }
            case LightComponent::SpotLight: {
                break;
            }
            default: {
                break;
            }
            }
        }
    }

    void on_destroy_entity(Entity entity)
    {
        auto &provider = get_provider();
        if (provider.directional_light == entity)
        {
            provider.directional_light = INVALID_ENTITY;
        }
        if (provider.point_lights.find(entity) != provider.point_lights.end())
        {
            provider.point_lights.erase(entity);
        }
    }

} // namespace Vultr::LightSystem

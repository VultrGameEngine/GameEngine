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
        register_global_system<Component>(signature, on_create_entity, nullptr);
    }

    void on_create_entity(Entity entity)
    {
        auto &provider = get_provider();
        provider.light = entity;
    }

} // namespace Vultr::LightSystem

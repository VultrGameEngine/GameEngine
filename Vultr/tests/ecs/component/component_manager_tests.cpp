#include <gtest/gtest.h>
#define private public
#define protected public

#include <ecs/component/component_manager.hpp>
#include <ecs/component/component_registry.hpp>
#include <core/components/transform_component.h>
#include <json/json.hpp>

using namespace Vultr;
TEST(ComponentManager, Serialization)
{
    ComponentManager manager;
    ComponentRegistry registry;
    component_registry_register_component<TransformComponent>(registry, [](Entity entity) {});
    component_manager_get_component_array<TransformComponent>(manager, component_registry_get_component_type<TransformComponent>(registry))
        ->InsertData(200, TransformComponent::Create(Vec3(29234, 2349, 10909)));
    component_manager_get_component_array<TransformComponent>(manager, component_registry_get_component_type<TransformComponent>(registry))->InsertData(39, TransformComponent::Create());
    component_manager_get_component_array<TransformComponent>(manager, component_registry_get_component_type<TransformComponent>(registry))->InsertData(87, TransformComponent::Create());
    component_manager_get_component_array<TransformComponent>(manager, component_registry_get_component_type<TransformComponent>(registry))->EntityDestroyed(200);
    json j;
    component_manager_to_json(j, manager, registry);
    std::cout << j.dump(4) << std::endl;
    ComponentManager retreieved_manager;
    component_manager_get_component_array<TransformComponent>(retreieved_manager, component_registry_get_component_type<TransformComponent>(registry));
    component_manager_from_json(j, retreieved_manager, registry);
    json retreived_j;
    component_manager_to_json(retreived_j, retreieved_manager, registry);
    std::cout << retreived_j.dump(4) << std::endl;
}

#include <gtest/gtest.h>
#define private public
#define protected public

#include <ecs/component/component_registry.hpp>
#include <core/components/transform_component.h>

using namespace Vultr;
TEST(ComponentRegistry, Serialization)
{
    ComponentRegistry registry;
    component_registry_register_component<TransformComponent>(registry, [](Entity entity) {});
    json j = registry;
    std::cout << j.dump(4) << std::endl;
}

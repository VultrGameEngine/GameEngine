#include <gtest/gtest.h>
#define private public
#define protected public

#include <ecs/entity/entity_manager.hpp>

using namespace Vultr;
TEST(EntityManager, Serialization)
{
    EntityManager manager;

    for (int i = 0; i < 10; i++)
    {
        entity_manager_create_entity(manager);
    }
    entity_manager_destroy_entity(manager, 3);
    entity_manager_destroy_entity(manager, 7);
    json j = manager;
    std::cout << j.dump(2) << std::endl;
}

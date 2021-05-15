#include <systems/test_system.h>
#include <components/test_component.h>

namespace TestSystem
{

    void register_system()
    {
        Signature signature;
        signature.set(get_component_type<TestComponent>(), true);
        world_register_system<Component>(signature, on_create_entity, nullptr);
    }

    void on_create_entity(Entity entity)
    {
        std::cout << "Test system on create entity" << std::endl;
    }
} // namespace TestSystem

#include <systems/test_system.h>
#include <components/test_component.h>

void TestSystem::RegisterSystem()
{
    Signature signature;
    signature.set(
        Vultr::Engine::GetComponentRegistry().GetComponentType<TestComponent>(),
        true);
    World::Get()->RegisterSystem<TestSystemProvider>(signature);
}

void TestSystem::OnCreateEntity(Entity entity)
{
    std::cout << "Test system on create entity" << std::endl;
}

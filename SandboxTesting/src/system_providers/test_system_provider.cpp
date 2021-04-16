#include <system_providers/test_system_provider.h>

void TestSystemProvider::OnCreateEntity(Entity entity)
{
    std::cout << "Test on create entity" << std::endl;
}

void TestSystemProvider::OnDestroyEntity(Entity entity)
{
    std::cout << "Test on destroy entity" << std::endl;
}

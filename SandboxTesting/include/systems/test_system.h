#pragma once
#include <vultr.hpp>
#include <system_providers/test_system_provider.h>

namespace TestSystem
{
    void register_system();
    void on_create_entity(Entity entity);
}; // namespace TestSystem

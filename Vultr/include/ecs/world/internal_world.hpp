#pragma once
#include "../component/component_manager.hpp"
#include "../entity/entity_manager.hpp"
#include "../system/system_manager.hpp"

namespace Vultr
{
    struct InternalWorld
    {
        ComponentManager component_manager;
        EntityManager entity_manager;
        SystemManager system_manager;
    };
} // namespace Vultr

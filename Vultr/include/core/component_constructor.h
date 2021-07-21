#pragma once
#include <ecs/entity/entity.hpp>

// Forward declaration
namespace Vultr
{
    struct Engine;
}

typedef void (*ComponentConstructor)(Vultr::Engine *e, Vultr::Entity);

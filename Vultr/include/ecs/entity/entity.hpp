#pragma once
#include <types/types.hpp>

namespace Vultr
{
// An entity is just an ID
typedef u32 Entity;

// Used to define the size of the arrays to hold all of these entities
const Entity MAX_ENTITIES = 100000;
} // namespace Vultr

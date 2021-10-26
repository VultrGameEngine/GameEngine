#pragma once
#include <types/types.h>

namespace Vultr
{
    // An entity is just an ID
    typedef u32 Entity;

    // Used to define the size of the arrays to hold all of these entities
    const Entity MAX_ENTITIES = 10000;

#define INVALID_ENTITY 0

} // namespace Vultr

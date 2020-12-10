#pragma once
#include <stdint.h>

// Type alias for entity
// An entity is just an ID, so we can just set it as a uint32_t
using Entity = uint32_t;

// Used to define the size of the arrays to hold all of these entities
const Entity MAX_ENTITIES = 5000;

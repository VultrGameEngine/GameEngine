#pragma once
#include <stdint.h>
#include <bitset>
#define VCOMPONENT(...)
#define VSYSTEM(...)
#define VSTRUCT(...)
#define VPROPERTY(...)
#define VFUNCTION(...)

// Alias to describe the ID of the components
// Every component has an ID which is just a number so we define [ComponentType] as
// just a uint8_t
using ComponentType = uint8_t;

// Defines the number of components we create
const ComponentType MAX_COMPONENTS = 32;

// Type alias for the components that an entity has
// If an entity uses components of type 0, 1, and 2 then the signature will be 0b111
// (bits 0, 1, and 2 are set) When we want to determine if an entity has all of the
// components required, then we use this signature to determine that The bitwise
// operation will make it so that the entity can have more components, but it
// definitely HAS to have these specific components of this [ComponentType]
using Signature = std::bitset<MAX_COMPONENTS>;

#pragma once
#include <stdint.h>
#include <bitset>
#define VCOMPONENT(...)
#define VSYSTEM(...)
#define VSTRUCT(...)
#define VPROPERTY(...)
#define VFUNCTION(...)

namespace Vultr
{
    // Alias to describe the ID of the components
    // Every component has an ID which is just a number so we define [ComponentType]
    // as just a uint32_t
    //
    // This will be set at compile time
    typedef uint32_t ComponentType;

    // Defines the number of components we create
    const ComponentType MAX_COMPONENTS = 32;

    // Type alias for the components that an entity has
    // If an entity uses components of type 0, 1, and 2 then the signature will be
    // 0b111 (bits 0, 1, and 2 are set) When we want to determine if an entity has
    // all of the components required, then we use this signature to determine that
    // The bitwise operation will make it so that the entity can have more
    // components, but it definitely HAS to have these specific components of this
    // [ComponentType]
    typedef std::bitset<MAX_COMPONENTS> Signature;

    inline bool signature_contains(Signature main, Signature match)
    {
        return (match & main) == main;
    }

    // Compile time hash
    constexpr uint32_t Hash32_CT(const char *str, size_t n,
                                 uint32_t basis = UINT32_C(2166136261))
    {
        return n == 0 ? basis
                      : Hash32_CT(str + 1, n - 1,
                                  (basis ^ str[0]) * UINT32_C(16777619));
    }

    template <uint32_t id>
    ComponentType typeid_helper()
    {
        return id;
    }
#define hash_type(T) typeid_helper<Hash32_CT(#T, sizeof(#T) - 1)>()
#define get_component_type(T) hash_type(T)
} // namespace Vultr

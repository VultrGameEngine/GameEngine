#pragma once
#include <stdint.h>
#include <bitset>
#include <types/types.h>
#include <json/json_fwd.hpp>
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

} // namespace Vultr

namespace nlohmann
{
    template <>
    struct adl_serializer<Vultr::Signature>
    {
        static void to_json(json &j, const Vultr::Signature &v);

        static void from_json(const json &j, Vultr::Signature &v);
    };
} // namespace nlohmann

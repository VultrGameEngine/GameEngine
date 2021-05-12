#pragma once
#include <types/types.hpp>

namespace Vultr
{
    // Compile time hash
    constexpr u32 Hash32_CT(const char *str, size_t n,
                            uint32_t basis = UINT32_C(2166136261))
    {
        return n == 0 ? basis
                      : Hash32_CT(str + 1, n - 1,
                                  (basis ^ str[0]) * UINT32_C(16777619));
    }

    template <u32 id>
    u32 typeid_helper()
    {
        return id;
    }

    template <typename T>
    const char *get_struct_name()
    {
        static_assert(
            !sizeof(T *),
            "Cannot get type info! You must register with `register_struct_ti(T)`. "
            "This function can only be used with structs");
    }

    template <typename T>
    const u32 hash_struct();

// Register the type info (stringified name of the struct and the hash) for a struct
#define register_struct_ti(T)                                                       \
    template <>                                                                     \
    inline const char *get_struct_name<T>()                                         \
    {                                                                               \
        return #T;                                                                  \
    }                                                                               \
    template <>                                                                     \
    inline const u32 hash_struct<T>()                                               \
    {                                                                               \
        return typeid_helper<Hash32_CT(#T, sizeof(#T) - 1)>();                      \
    }
} // namespace Vultr

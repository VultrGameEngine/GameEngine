#pragma once
#include <types/types.hpp>

namespace Vultr
{
    // Compile time hash
    constexpr u32 Hash32_CT(const char *str, size_t n, uint32_t basis = UINT32_C(2166136261))
    {
        return n == 0 ? basis : Hash32_CT(str + 1, n - 1, (basis ^ str[0]) * UINT32_C(16777619));
    }

    template <u32 id>
    u32 typeid_helper()
    {
        return id;
    }

    template <typename T>
    const char *get_struct_name();
} // namespace Vultr

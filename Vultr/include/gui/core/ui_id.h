#pragma once
#include <types/types.h>
#include <math/crc32.h>

namespace Vultr
{
    namespace IMGUI
    {
        typedef u32 UI_ID;
        typedef u32 Widget_ID;

// To make __LINE__ expand to a string, use S__LINE__
#define S(x) #x
#define S_(x) S(x)
#define S__LINE__ S_(__LINE__)

#define AUTO_ID() CRC32_STR(__FILE__ S__LINE__)
#define NO_ID() 0xFFFFFFFF
#define UNSET_ID() 0x0
    } // namespace IMGUI
} // namespace Vultr

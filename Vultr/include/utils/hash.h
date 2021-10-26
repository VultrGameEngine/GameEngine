#pragma once
#include <types/types.h>

namespace Vultr
{
    typedef u32 Hash;
    consteval Hash hash_id(const char *value)
    {
        s32 i, j;
        u32 byte, crc, mask;

        i = 0;
        crc = 0xFFFFFFFF;
        while (value[i] != 0)
        {
            byte = value[i]; // Get next byte.
            crc = crc ^ byte;
            for (j = 7; j >= 0; j--)
            { // Do eight times.
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
            i = i + 1;
        }
        return ~crc;
    }

    inline Hash runtime_hash_id(const char *value)
    {
        s32 i, j;
        u32 byte, crc, mask;

        i = 0;
        crc = 0xFFFFFFFF;
        while (value[i] != 0)
        {
            byte = value[i]; // Get next byte.
            crc = crc ^ byte;
            for (j = 7; j >= 0; j--)
            { // Do eight times.
                mask = -(crc & 1);
                crc = (crc >> 1) ^ (0xEDB88320 & mask);
            }
            i = i + 1;
        }
        return ~crc;
    }
} // namespace Vultr

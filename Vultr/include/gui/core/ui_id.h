#pragma once
#include <types/types.h>

namespace Vultr
{
    namespace IMGUI
    {
        typedef s32 UI_ID;
        typedef s32 Widget_ID;

#define AUTO_ID Vultr::IMGUI::ui_id(__FILE__ + std::to_string(__LINE__))
#define NO_ID -1
#define UNSET_ID -2

        consteval UI_ID ui_id(const char *value)
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
    } // namespace IMGUI
} // namespace Vultr

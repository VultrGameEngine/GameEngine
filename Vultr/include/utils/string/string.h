#pragma once

namespace Vultr
{
    char *str(size_t len);
    char *restr(char *str, size_t len);
    char *str(const char *string);
    char *strn(const char *string, size_t len);

    bool strequal(const char *str1, const char *str2);
    char *strappend(char *dest, const char *src);
} // namespace Vultr

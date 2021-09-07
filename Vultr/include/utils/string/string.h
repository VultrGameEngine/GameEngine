#pragma once

namespace Vultr
{
    char *str(size_t len);
    char *restr(char *str, size_t len);
    char *str(const char *string);
    char *strn(const char *string, size_t len);

    // Replace all occurrences of a character in a string
    void strcreplace(char *string, char src, char dest);

    // Replace all occurrences of a string in a string
    char *strsreplace(char *string, const char *src, const char *dest);

    bool strequal(const char *str1, const char *str2);
    char *strappend(char *dest, const char *src);
} // namespace Vultr

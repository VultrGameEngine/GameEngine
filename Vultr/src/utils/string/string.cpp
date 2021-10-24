#include <utils/string/string.h>
#include <types/types.hpp>

namespace Vultr
{
    char *str(size_t len)
    {
        size_t buf_size = sizeof(char) * (len + 1);
        char *str = static_cast<char *>(malloc(buf_size));
        assert(str != nullptr && "Failed to allocate string!");

        // Initialize the memory to 0 so that we don't have to worry about garbage string data
        memset(str, 0, buf_size);
        return str;
    }

    char *restr(char *string, size_t len)
    {
        if (string == nullptr)
        {
            return str(len);
        }
        char *restr = static_cast<char *>(realloc(string, sizeof(char) * (len + 1)));
        assert(restr != nullptr && "Failed to allocate string!");
        return restr;
    }

    char *str(const char *string)
    {
        return strdup(string);
    }

    char *strn(const char *string, size_t len)
    {
        char *allocated_string = str(len);
        strncpy(allocated_string, string, len);
        return allocated_string;
    }

    bool strequal(const char *str1, const char *str2)
    {
        if (str1 == nullptr && str2 == nullptr)
            return true;
        if (str1 == nullptr)
            return false;
        if (str2 == nullptr)
            return false;

        return strcmp(str1, str2) == 0;
    }

    bool strnequal(const char *str1, const char *str2, size_t len)
    {
        if (str1 == nullptr && str2 == nullptr)
            return true;
        if (str1 == nullptr)
            return false;
        if (str2 == nullptr)
            return false;
        for (size_t i = 0; i < len; i++)
        {
            if (str1[i] != str2[i])
                return false;
        }
        return true;
    }

    char *strappend(char *dest, const char *src)
    {
        size_t len = strlen(dest) + strlen(src);

        if (dest == nullptr)
        {
            dest = str(len);
        }
        else
        {
            dest = restr(dest, len);
        }

        strcat(dest, src);
        return dest;
    }

    void strcreplace(char *string, char src, char dest)
    {
        size_t len = strlen(string);
        for (u32 i = 0; i < len; i++)
        {
            if (string[i] == src)
            {
                string[i] = dest;
            }
        }
    }

    // char *strsreplace(char *string, const char *src, const char *dest)
    // {
    //     throw("Not implemented");
    // }
} // namespace Vultr

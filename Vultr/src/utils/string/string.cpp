#include <utils/string/string.h>

namespace Vultr
{
    char *str(size_t len)
    {
        return static_cast<char *>(malloc(sizeof(char) * (len + 1)));
    }

    char *str(const char *string)
    {
        size_t len = strlen(string);
        char *allocated_string = str(len);
        strcpy(allocated_string, string);
        return allocated_string;
    }
} // namespace Vultr

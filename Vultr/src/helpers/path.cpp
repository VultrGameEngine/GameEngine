#include <helpers/path.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

namespace Vultr::Path
{
std::string GetFullPath(const std::string &path)
{

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char buff[256];
    size_t len = sizeof(buff);
    int bytes = GetModuleFileName(NULL, buff, len);
    if (bytes != -1)
    {
        buff[bytes] = '\0';
        return std::string(buff) + path;
    }
#else
    char buff[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
    if (len != -1)
    {
        buff[len] = '\0';
        return std::string(buff) + path;
    }
#endif
    assert(false && "COULD NOT GET EXE PATH");
    return std::string("ERROR");
}
} // namespace Vultr::Path

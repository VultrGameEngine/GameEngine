#include <helpers/path.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <limits.h>
#include <assert.h>

namespace Vultr::Path
{
std::string GetWindowsPath(const std::string& path) {
    std::string windows_path = "";
    for (char c : path) {
        if (c == '/') {
            windows_path += "\\";
        }
        else
        {
            windows_path += c;
        }
    }
    return windows_path;
}

std::string GetFullPath(const std::string &path)
{
#ifdef ENABLE_DEBUG_MACRO
#ifdef _WIN32
    return GetWindowsPath("C:/Users/Brand/Dev/GameEngine/SandboxTesting/" + path);
#else
    return "/home/brandon/Dev/GameEngine/SandboxTesting/" + path;
#endif
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    char buff[256];
    ssize_t len = GetModuleFileName(NULL, buff, sizeof(buff));
#else
    char buff[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
#endif
    if (len != -1)
    {
        buff[len] = '\0';
        std::string base = std::string(buff);
        std::string full_path = base.replace(base.rfind("Game"), len, "") + path;
        return full_path;
    }
    assert(false && "COULD NOT GET EXE PATH");
    return std::string("ERROR");
#endif
}
} // namespace Vultr::Path

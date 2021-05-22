#include <helpers/path.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <limits.h>
#include <assert.h>

namespace Vultr::Path
{
    std::string GetWindowsPath(const std::string &path)
    {
        std::string windows_path = "";
        for (char c : path)
        {
            if (c == '/')
            {
                windows_path += "\\";
            }
            else
            {
                windows_path += c;
            }
        }
        return windows_path;
    }

    void set_resource_path(const std::string &path)
    {
        get_resource_path() = path;
    }

    std::string &get_resource_path()
    {
        static std::string resource_path = "";
        return resource_path;
    }

    std::string get_shortened_resource_path(const std::string &path)
    {
        if (path.size() < get_resource_path().size())
            return path;
        if (path.substr(0, get_resource_path().size()) == get_resource_path())
            return path.substr(get_resource_path().size());
        return path;
    }

    std::string GetFullPath(const std::string &path)
    {
// #ifdef ENABLE_DEBUG_MACRO
#ifdef _WIN32
        return GetWindowsPath(get_resource_path() + path);
#else
        return get_resource_path() + path;
#endif
        // #else
        // #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        //         char buff[256];
        //         DWORD len = GetModuleFileName(NULL, buff, sizeof(buff));
        // #else
        //         char buff[PATH_MAX];
        //         ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1);
        // #endif
        //         if (len != -1)
        //         {
        //             buff[len] = '\0';
        //             std::string base = std::string(buff);
        //             std::string full_path = base.replace(base.rfind("Game"), len, "") + path;
        //             return full_path;
        //         }
        //         assert(false && "COULD NOT GET EXE PATH");
        //         return std::string("ERROR");
        // #endif
    }
} // namespace Vultr::Path

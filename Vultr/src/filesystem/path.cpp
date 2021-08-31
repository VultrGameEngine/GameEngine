#include <filesystem/path.h>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <types/types.hpp>

namespace Vultr
{
    void change_working_directory(const char *dir)
    {
        chdir(dir);
    }

    void get_working_directory(char *buf, size_t size)
    {
        assert(getcwd(buf, size) != nullptr && "Failed to get current working directory!");
    }
} // namespace Vultr

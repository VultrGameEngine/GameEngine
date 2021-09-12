#include <services/reload_watcher.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

using namespace Vultr;

static u64 get_time()
{
    struct timeval time;
    gettimeofday(&time, nullptr);

    u64 ms = (u64)time.tv_sec * 1000 + (u64)time.tv_usec / 1000;
    return ms;
}

ReloadWatcher *init_reload_watcher(DLLSource *src, u64 check_interval_miliseconds)
{
    auto *watcher = new ReloadWatcher();
    watcher->dll = *src;
    watcher->check_interval_miliseconds = check_interval_miliseconds;
    watcher->last_check_time = get_time();
    watcher->last_load_time = get_time();
    return watcher;
}

bool watch_dll(ReloadWatcher *watcher)
{
    auto time = get_time();
    struct stat statbuf;
    stat(watcher->dll.path, &statbuf);
    auto ms = (u64)statbuf.st_ctim.tv_sec * 1000;
    if (time - ms > 2000 && ms > watcher->last_load_time + 2000)
    {
        fprintf(stdout, "RELOADING!\n");
        watcher->last_load_time = ms;
        return true;
    }
    return false;
}

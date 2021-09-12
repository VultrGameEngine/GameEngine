#pragma once
#include <vultr.hpp>

struct ReloadWatcher
{
    Vultr::DLLSource dll;

    u64 check_interval_miliseconds;
    u64 last_load_time = 0;
    u64 last_check_time = 0;
};

ReloadWatcher *init_reload_watcher(Vultr::DLLSource *dll, u64 check_interval_miliseconds = 500);
bool watch_dll(ReloadWatcher *watcher);

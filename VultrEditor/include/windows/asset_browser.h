#pragma once
#include <editor.h>

struct AssetBrowser
{
    Vultr::Directory current_directory;

    // Selected file/directory as an index
    s32 selected = -1;
};

void register_asset_browser(const Vultr::Directory &current_dir);
void asset_browser_render(const Vultr::UpdateTick &tick, void *state);

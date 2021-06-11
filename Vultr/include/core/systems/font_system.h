#pragma once
#include <helpers/file.h>

namespace Vultr::FontSystem
{
    void register_system();
    void init();
    void preload_font(const FontSource &source);
} // namespace Vultr::FontSystem

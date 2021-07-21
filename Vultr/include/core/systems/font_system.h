#pragma once
#include <engine.hpp>
#include <helpers/file.h>

namespace Vultr::FontSystem
{
    void register_system(Engine *e);
    void init(Engine *e);
    void preload_font(Engine *e, const FontSource &source);
} // namespace Vultr::FontSystem

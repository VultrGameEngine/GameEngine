#pragma once
#include <types/types.hpp>
#include <core/models/update_tick.h>

namespace Vultr
{
    struct Engine;
}

struct Scene
{
    virtual void init(Vultr::Engine *e) = 0;
    virtual void update(Vultr::Engine *e, const Vultr::UpdateTick &tick) = 0;
    virtual void flush(Vultr::Engine *e) = 0;
    virtual ~Scene(){};
};

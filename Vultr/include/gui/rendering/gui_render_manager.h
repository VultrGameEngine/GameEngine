#pragma once
#include "gui_render_group.h"

namespace Vultr
{
namespace GUI
{
struct ZRange
{
    Zindex min;
    Zindex max;
    bool operator=(const ZRange &o)
    {
        return min == o.min && max == o.max;
    }
    bool operator<(const ZRange &o) const
    {
        return max < o.min;
    }
};

class GUIRenderManager
{
  public:
  private:
    std::map<ZRange, RenderGroup *> gui_render_groups;
};
} // namespace GUI
} // namespace Vultr

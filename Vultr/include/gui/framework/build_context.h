#pragma once
#include <gui/rendering/gui_render_group.h>
#include <vector>

namespace Vultr
{
namespace GUI
{

class BuildContext
{
  public:
    BuildContext()
    {
        groups.push_back(RenderGroup());
    }
    // Our starting accumulators
    struct Accumulator
    {
        glm::vec2 size = glm::vec2(2, 2);
        glm::vec2 position = glm::vec2(0, 0);
    } accumulator;

    Quad GetQuad(int index, int layer = 0)
    {
        RenderGroup &group = groups[layer];
        return group.GetQuadAtIndex(index);
    }

    bool DeleteQuad(int index, int layer = 0)
    {
        RenderGroup &group = groups[layer];
        return group.DeleteQuadAtIndex(index);
    }

    int SubmitQuad(int layer = 0)
    {
        RenderGroup &group = groups[layer];
        return group.SubmitQuad();
    }

    void Draw()
    {
        RenderGroup &group = groups[0];
        group.Draw();
    }

    Accumulator GetCurrentDimensions() const
    {
        return accumulator;
    }

    void AccumulatePosition(glm::vec2 position)
    {
        accumulator.position = position;
    }
    void AccumulateSize(glm::vec2 size)
    {
        accumulator.size = size;
    }

  private:
    std::vector<RenderGroup> groups;
};
} // namespace GUI
} // namespace Vultr

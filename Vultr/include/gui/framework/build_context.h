#pragma once
#include <gui/rendering/gui_render_group.h>
#include <vector>
#include <stack>

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
        positions.push(glm::vec2(0, 0));
        zindex.push(0);
    }

    std::stack<glm::vec2> positions;
    std::stack<int> zindex;

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

    int SubmitQuad()
    {
        for (int i = 0; i < groups.size(); i++)
        {
            RenderGroup &group = groups[i];
            int res = group.SubmitQuad();
            if (res != -1)
                return res;
        }
        RenderGroup new_group = RenderGroup();
        groups.push_back(new_group);
        return new_group.SubmitQuad();
    }

    void Draw()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        for (RenderGroup &group : groups)
            group.Draw();

        zindex.empty();
        zindex.push(0);
        positions.empty();
        positions.push(glm::vec2(0, 0));
    }

    glm::vec2 GetPosition() const
    {
        return positions.top();
    }

    void ExitBranch()
    {
        assert(positions.size() > 1 && zindex.size() > 1 && "Cannot pop!");
        positions.pop();
        zindex.pop();
    }

    void Branch()
    {
        positions.push(positions.top());
        zindex.push(zindex.top());
    }

    void AccumulatePosition(glm::vec2 position, bool branch = false)
    {
        positions.top() += position;
    }

    int IncreaseZ()
    {
        return zindex.top()--;
    }

  private:
    std::vector<RenderGroup> groups;
};
} // namespace GUI
} // namespace Vultr

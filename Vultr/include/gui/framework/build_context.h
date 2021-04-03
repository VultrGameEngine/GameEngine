#pragma once
#include <gui/rendering/gui_render_group.h>
#include <vector>
#include <stack>
#include <fonts/font.h>
#include <helpers/font_importer.h>
#include <core/system_providers/font_system_provider.h>
#include <core/system_providers/input_system_provider.h>
#include "input_receiver.h"
#include <core/models/update_tick.h>

namespace Vultr
{
namespace GUI
{

class BuildContext
{
  public:
    BuildContext() : tick_info(UpdateTick(0, false))
    {
        groups.push_back(new RenderGroup());
        positions.push(glm::vec2(0, 0));
        zindex.push(0);
    }

    UpdateTick GetTickInfo()
    {
        return tick_info;
    }

    void PassUpdate(UpdateTick tick)
    {
        tick_info = tick;
    }

    std::stack<glm::vec2> positions;
    std::stack<int> zindex;

    Quad GetQuad(QuadID quad, int layer = 0)
    {
        RenderGroup *group = groups[layer];
        return group->GetQuad(quad);
    }

    bool DeleteQuad(QuadID quad, int layer = 0)
    {
        RenderGroup *group = groups[layer];
        return group->DeleteQuad(quad);
    }

    QuadID SubmitQuad(Zindex z_index, Texture *texture = nullptr)
    {
        for (int i = 0; i < groups.size(); i++)
        {
            RenderGroup *group = groups[i];
            QuadID res = group->SubmitQuad(z_index, texture);
            if (res != -1)
                return res;
        }
        RenderGroup *new_group = new RenderGroup();
        groups.push_back(new_group);
        return new_group->SubmitQuad(z_index, texture);
    }

    Font *GetFont(const std::string &path)
    {
        if (fonts.find(path) != fonts.end())
        {
            return fonts[path];
        }
        fonts[path] =
            FontImporter::ImportFont(path, FontSystemProvider::Get()->library);
        return fonts[path];
    }

    // QuadID SubmitQuad(std::string texture)
    // {
    // }

    QuadID SubmitQuad(Zindex z_index, Font *font)
    {
        return SubmitQuad(z_index, font->texture);
    }

    void SubmitRenderGroup(RenderGroup* group) 
    {
        
    }

    void CommitQuad(QuadID id, Zindex z_index) 
    {
        RenderGroup *group = groups[0];
        group->CommitQuad(id, z_index);
    }

    void Draw(Shader *shader)
    {
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        for (RenderGroup *group : groups)
            group->Draw(shader);

        for (int i = 0; i < zindex.size(); i++)
        {
            zindex.pop();
        }
        for (int i = 0; i < positions.size(); i++)
        {
            positions.pop();
        }
        zindex.push(0);
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

    void SetPosition(glm::vec2 position)
    {
        positions.top() = position;
    }

    int IncreaseZ()
    {
        return zindex.top()--;
    }

    void SubmitInputReceiver(int p_zindex, InputReceiver *receiver)
    {
        input_receivers[-p_zindex] = receiver;
    }

    void DeleteInputReceiver(int p_zindex)
    {
        assert(input_receivers.find(-p_zindex) == input_receivers.end() &&
               "Could not delete input receiver");
        input_receivers.erase(-p_zindex);
    }

    bool SubmitMouseInputEvent(Input::MouseInputEvent event)
    {
        for (auto [z, receiver] : input_receivers)
        {
            if (receiver->ReceiveMouseEvent(event))
                return true;
        }
        return false;
    }
    bool SubmitMouseButtonInputEvent(Input::MouseButtonInputEvent event)
    {
        for (auto [z, receiver] : input_receivers)
        {
            if (receiver->ReceiveMouseButtonEvent(event))
                return true;
        }
        return false;
    }
    bool SubmitScrollInputEvent(Input::ScrollInputEvent event)
    {
        for (auto [z, receiver] : input_receivers)
        {
            if (receiver->ReceiveScrollEvent(event))
                return true;
        }
        return false;
    }

  private:
    std::vector<RenderGroup *> groups;
    std::unordered_map<std::string, Font *> fonts;
    std::map<unsigned int, InputReceiver *> input_receivers;
    UpdateTick tick_info;
};
} // namespace GUI
} // namespace Vultr

#pragma once
#include <gui/rendering/gui_vertex.h>
#include "build_context.h"
#include "box_constraints.h"

namespace Vultr
{
namespace GUI
{
class Widget;

class RenderObject
{
  public:
    RenderObject(Widget *widget) : configuration(widget)
    {
    }

    virtual void DeleteRenderObject(BuildContext *context)
    {
        if (quad_id != -1)
        {
            context->DeleteQuad(quad_id);
        }
    }

    virtual ~RenderObject()
    {
    }

    virtual void Paint(BuildContext *context) = 0;

    virtual Size Layout(BuildContext *context, BoxConstraints constraints) = 0;

    // This is only used so that if you want to lay out a flex widget
    virtual Size GetSize(BuildContext *context, BoxConstraints constraints)
    {
        return Layout(context, constraints);
    }

    void MarkForRepaint()
    {
        repaint_required = true;
    }

    bool NeedsRepaint()
    {
        return repaint_required;
    }

    void Reattach(Widget *new_config)
    {
        this->configuration = new_config;
        MarkForRepaint();
    }

  protected:
    bool repaint_required = true;
    QuadID quad_id = -1;
    Widget *configuration;
    QuadProperties properties;

    virtual Widget *GetConfig()
    {
        return configuration;
    }

    Quad GetQuad(BuildContext *context, int index, int layer = 0)
    {
        return context->GetQuad(index, layer);
    }

    Size GetSize()
    {
        return size;
    }

    Size UpdateSize(Size size)
    {
        if (this->size != size)
            MarkForRepaint();
        this->size = size;
        return this->size;
    }

  private:
    Size size;
};
} // namespace GUI
} // namespace Vultr

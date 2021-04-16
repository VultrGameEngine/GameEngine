#pragma once
#include "single_child_render_object_widget.h"

namespace Vultr
{
namespace GUI
{
class WindowElement : public SingleChildElement
{
  public:
    WindowElement(BuildContext *context, SingleChildRenderObjectWidget *p_widget)
        : SingleChildElement(context, p_widget)
    {
    }

    void Layout(BuildContext *context, Size size)
    {
        child->Layout(context, BoxConstraints::Tight(size));
    }
};

class Window : public SingleChildRenderObjectWidget
{
  private:
    class RenderedWindow : public SingleChildRenderObject
    {
      public:
        RenderedWindow(BuildContext *context, Widget *widget)
            : SingleChildRenderObject(widget)
        {
            // TODO Fix this so that the layer is dependent on that given by the
            // parent
            //quad_id = context->SubmitQuad();
        }

        Window *GetConfig() override
        {
            return (Window *)configuration;
        }

        void Paint(BuildContext *context) override
        {
            repaint_required = false;
            context->AccumulatePosition(position);
        }

        Size Layout(BuildContext *context, BoxConstraints constraints,
                    Element *child) override
        {
            return Size(0, 0);
        }
    };

  public:
    Window(Widget *p_child)
    {
        this->child = p_child;
    }

    WindowElement *CreateElement(BuildContext *context) override
    {
        return new WindowElement(context, this);
    }

    SingleChildRenderObject *CreateRenderObject(BuildContext *context) override
    {
        return (SingleChildRenderObject *)new RenderedWindow(context, this);
    }
};
} // namespace GUI
} // namespace Vultr

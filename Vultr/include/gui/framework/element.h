#pragma once
#include "widget.h"
#include <vector>
#include "render_object_widget.h"

namespace Vultr
{
namespace GUI
{
class Element
{
  public:
    Element(BuildContext *context, Widget *p_widget) : widget(p_widget)
    {
    }

    virtual void DeleteElement(BuildContext *context) = 0;

    virtual void Rebuild(BuildContext *context) = 0;

    virtual Size Layout(BuildContext *context, BoxConstraints constraints) = 0;

    // This is only used so that if you want to lay out a flex widget
    virtual Size GetSize(BuildContext *context, BoxConstraints constraints)
    {
        return Layout(context, constraints);
    }

    virtual void Update(BuildContext *context) = 0;

    virtual Widget *GetWidget() const
    {
        return widget;
    }

    virtual void Reattach(Widget *self)
    {
        this->widget = self;
    }

  protected:
    Widget *widget;
    virtual ~Element()
    {
        delete widget;
    }
};

class RenderObjectElement : public Element
{
  public:
    RenderObjectElement(BuildContext *context, RenderObjectWidget *p_widget)
        : Element(context, p_widget)
    {
        render_object = GetWidget()->CreateRenderObject(context);
    }

    virtual void DeleteElement(BuildContext *context) override
    {
        render_object->DeleteRenderObject(context);
        delete this;
    }

    virtual void Reattach(Widget *self) override
    {
        this->widget = self;
        this->render_object->Reattach(this->widget);
    }

    virtual void Update(BuildContext *context) override
    {
        if (render_object->NeedsRepaint())
        {
            render_object->Paint(context);
        }
    }

    virtual Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        // By default fill up the maximum area
        return render_object->Layout(context, constraints);
    }

    virtual RenderObjectWidget *GetWidget() const override
    {
        return (RenderObjectWidget *)widget;
    }

  protected:
    RenderObject *render_object;
    virtual ~RenderObjectElement()
    {
        delete render_object;
    }
};
} // namespace GUI
} // namespace Vultr

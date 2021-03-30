#pragma once
#include "render_object_widget.h"
#include "element.h"

namespace Vultr
{
namespace GUI
{

class SingleChildRenderObject : public RenderObject
{
  public:
    SingleChildRenderObject(Widget *widget) : RenderObject(widget)
    {
    }

    virtual ~SingleChildRenderObject()
    {
    }

    virtual Size Layout(BuildContext *context, BoxConstraints constraints,
                        Element *child) = 0;

    virtual Size Layout(BuildContext *context, BoxConstraints constraints,
                        Element *child, bool layout)
    {
        return Layout(context, constraints, child);
    }

  protected:
    glm::vec2 position;

  private:
    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert("Incorrect method called!");
        return constraints.Min();
    }
};

class SingleChildRenderObjectWidget : public RenderObjectWidget
{
  public:
    Element *CreateElement(BuildContext *context) override;
    virtual ~SingleChildRenderObjectWidget()
    {
    }

    Widget *GetChild()
    {
        return child;
    }
    WidgetTypeGetter(SingleChildRenderObjectWidget);

    virtual SingleChildRenderObject *CreateRenderObject(
        BuildContext *context) override = 0;

  protected:
    Widget *child;
};

class SingleChildElement : public RenderObjectElement
{
  public:
    SingleChildElement(BuildContext *context,
                       SingleChildRenderObjectWidget *p_widget)
        : RenderObjectElement(context, p_widget)
    {
    }

    void Update(BuildContext *context) override
    {
        if (render_object->NeedsRepaint())
        {
            render_object->Paint(context);
        }

        if (child != nullptr)
        {
            child->Update(context);
        }
    }

    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No render object attached to this element");
        return ((SingleChildRenderObject *)render_object)
            ->Layout(context, constraints, child);
    }

    // This is only used so that if you want to lay out a flex widget
    virtual Size GetSize(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No render object attached to this element");
        return ((SingleChildRenderObject *)render_object)
            ->Layout(context, constraints, child, false);
    }

    void Reattach(Widget *self) override
    {
        widget = self;
        render_object->Reattach(widget);
        render_object->MarkForRepaint();
    }

    void Rebuild(BuildContext *context) override
    {
        // What this element widget says the child is
        Widget *new_widget = GetWidget()->GetChild();
        if (new_widget != nullptr)
        {
            if (child != nullptr && child->GetWidget() != nullptr)
            {
                // The element's currently attached child element widget
                Widget *old_widget = child->GetWidget();

                // If the widget memory addresses are equal, that definitely means
                // nothing needs to be reattached or anything, don't touch or delete
                // anything
                if (old_widget != new_widget)
                {
                    // If we don't have to recreate the element, then don't
                    if (old_widget->GetType() == new_widget->GetType() &&
                        old_widget->key == new_widget->key)
                    {
                        // We can just delete the old widget of the child
                        delete old_widget;

                        // Add reattach the new update widget
                        child->Reattach(new_widget);
                    }
                    // Otherwise if the types don't match and we need to recreate,
                    // then delete our child since it obviously doesn't work
                    else
                    {
                        child->DeleteElement(context);
                    }
                }
            }
            else
            {
                // If the child elements don't match, then we need to recreate it
                // from our own widget
                child = new_widget->CreateElement(context);
            }
            // Continue down the tree
            child->Rebuild(context);
        }
    }

    virtual SingleChildRenderObjectWidget *GetWidget() const override
    {
        return (SingleChildRenderObjectWidget *)widget;
    }

  protected:
    Element *child = nullptr;
    virtual ~SingleChildElement()
    {
    }
};

} // namespace GUI
} // namespace Vultr

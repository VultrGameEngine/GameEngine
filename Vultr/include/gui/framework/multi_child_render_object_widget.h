#pragma once
#include "render_object_widget.h"
#include "element.h"

namespace Vultr
{
namespace GUI
{

class MultiChildRenderObject : public RenderObject
{
  public:
    MultiChildRenderObject(Widget *widget) : RenderObject(widget)
    {
    }

    virtual ~MultiChildRenderObject()
    {
    }

    virtual Size Layout(BuildContext *context, BoxConstraints constraints,
                        std::vector<Element *> children) = 0;

    virtual Size Layout(BuildContext *context, BoxConstraints constraints,
                        std::vector<Element *> children, bool layout)
    {
        return Layout(context, constraints, children);
    }

    virtual void ApplyPosition(BuildContext *context, int index) = 0;

  protected:
    std::vector<Vec2> positions;

  private:
    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert("Incorrect method called!");
        return constraints.Min();
    }
};

class MultiChildRenderObjectWidget : public RenderObjectWidget
{
  public:
    Element *CreateElement(BuildContext *context) override;

    const std::vector<Widget *> &GetChildren()
    {
        return children;
    }
    WidgetTypeGetter(MultiChildRenderObjectWidget);

    virtual MultiChildRenderObject *CreateRenderObject(
        BuildContext *context) override = 0;

  protected:
    std::vector<Widget *> children;
};

class MultiChildElement : public RenderObjectElement
{
  public:
    MultiChildElement(BuildContext *context, MultiChildRenderObjectWidget *p_widget)
        : RenderObjectElement(context, p_widget)
    {
    }

    void DeleteElement(BuildContext *context) override
    {
        render_object->DeleteRenderObject(context);
        for (Element *element : children)
        {
            element->DeleteElement(context);
        }
        delete this;
    }

    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No render object attached to this element");
        return ((MultiChildRenderObject *)render_object)
            ->Layout(context, constraints, children);
    }

    // This is only used so that if you want to lay out a flex widget
    virtual Size GetSize(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No render object attached to this element");
        return ((MultiChildRenderObject *)render_object)
            ->Layout(context, constraints, children, false);
    }

    void Update(BuildContext *context) override
    {
        if (render_object->NeedsRepaint(context))
        {
            render_object->Paint(context);
        }
        for (int i = 0; i < children.size(); i++)
        {
            Element *child = children.at(i);
            context->Branch();
            ((MultiChildRenderObject *)render_object)->ApplyPosition(context, i);
            child->Update(context);
            context->ExitBranch();
        }
    }

    void Reattach(Widget *self) override
    {
        widget = self;
        render_object->Reattach(widget);
        render_object->MarkForRepaint();
    }

    void Rebuild(BuildContext *context) override
    {
        int difference = children.size() - GetWidget()->GetChildren().size();
        if (difference == 1)
        {
            children.pop_back();
        }
        else if (difference > 0)
        {
            children.erase(children.end() - difference - 1, children.end());
        }
        for (int i = 0; i < GetWidget()->GetChildren().size(); i++)
        {
            // What this element widget says the child is
            Widget *new_widget = GetWidget()->GetChildren().at(i);

            // If the element at that index even exists here
            if (i < children.size())
            {
                // The element's currently attached child element widget
                Widget *old_widget = children.at(i)->GetWidget();

                // If the widget memory addresses are equal, that definitely means
                // nothing needs to be reattached or anything, don't touch or delete
                // anything
                if (old_widget != new_widget)
                {
                    if (old_widget != nullptr)
                    {

                        // If we don't have to recreate the element, then don't
                        if (old_widget->GetType() == new_widget->GetType() &&
                            old_widget->key == new_widget->key)
                        {
                            // We can just delete the old widget of the child
                            delete old_widget;

                            // Add reattach the new update widget
                            children.at(i)->Reattach(new_widget);
                        }
                        // Otherwise if the types don't match and we need to
                        // recreate, then delete our child since it obviously doesn't
                        // work and create from scratch
                        else
                        {
                            children.at(i)->DeleteElement(context);
                            children.at(i) = new_widget->CreateElement(context);
                        }
                    }
                    // If the widget of the child was never attached in the first
                    // place, then just delete and create from scratch
                    else
                    {
                        children.at(i)->DeleteElement(context);
                        children.at(i) = new_widget->CreateElement(context);
                    }
                }
            }
            // Otherwise if the index does not exist locally, we just create the
            // element and push it to the back
            else
            {
                children.push_back(new_widget->CreateElement(context));
            }

            // Once we are done updating / reattaching / creating the element, we
            // need to call the rebuild method of this child
            children.at(i)->Rebuild(context);
        }
    }

    virtual MultiChildRenderObjectWidget *GetWidget() const override
    {
        return (MultiChildRenderObjectWidget *)widget;
    }

  protected:
    virtual ~MultiChildElement()
    {
    }
    std::vector<Element *> children;
};
} // namespace GUI
} // namespace Vultr

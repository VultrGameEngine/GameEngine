#pragma once
#include "render_object_widget.h"
#include "element.h"
#include <vector>

namespace Vultr
{
namespace GUI
{

class MultiChildRenderObjectWidget : public RenderObjectWidget
{
  public:
    Element *CreateElement(BuildContext *context) override;

    const std::vector<Widget *> &GetChildren()
    {
        return children;
    }
    WidgetTypeGetter(MultiChildRenderObjectWidget);

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
        for (Element *element : children)
        {
            element->DeleteElement(context);
        }
        delete this;
    }

    void Update(BuildContext *context) override
    {
        if (render_object->NeedsRepaint())
        {
            render_object->Paint(context);
        }
        for (Element *child : children)
        {
            child->Update(context);
        }
    }

    void Rebuild(BuildContext *context) override
    {
        for (int i = 0; i < GetWidget()->GetChildren().size(); i++)
        {
            // What this element widget says the child is
            Widget *new_widget = GetWidget()->GetChildren().at(i);
            if (i < children.size())
            {
                // The element's currently attached child element widget
                Widget *old_widget = children.at(i)->GetWidget();

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
                // If the widget of the child was never attached in the first place,
                // then just delete and create from scratch
                else
                {
                    children.at(i)->DeleteElement(context);
                    children.at(i) = new_widget->CreateElement(context);
                }
            }
            else
            {
                assert(i == children.size() &&
                       "Something went wrong when rebuilding");
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

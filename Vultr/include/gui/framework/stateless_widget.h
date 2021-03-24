#pragma once
#include "widget.h"
#include "element.h"

namespace Vultr
{
namespace GUI
{
class StatelessWidget : public Widget
{
  public:
    virtual Widget *Build() = 0;
    Element *CreateElement(BuildContext *context) override;
    WidgetTypeGetter(StatelessWidget)
};

class StatelessElement : public Element
{
  public:
    StatelessElement(BuildContext *context, Widget *p_widget)
        : Element(context, p_widget)
    {
    }

    void DeleteElement(BuildContext *context) override
    {
        if (child != nullptr)
            child->DeleteElement(context);
        delete this;
    }

    void Update(BuildContext *context) override
    {
        if (child != nullptr)
        {
            child->Update(context);
        }
    }

    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert(child != nullptr && "No child for this stateless widget!");
        return child->Layout(context, constraints);
    }

    void Rebuild(BuildContext *context) override
    {
        Widget *build_res = GetWidget()->Build();
        if (child != nullptr && child->GetWidget() != nullptr)
        {
            // The element's currently attached child element widget
            Widget *old_widget = child->GetWidget();

            // What this element widget says the child is
            Widget *new_widget = build_res;

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
                // Otherwise if the types don't match and we need to recreate, then
                // delete our child since it obviously doesn't work
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
            child = build_res->CreateElement(context);
        }
        // Continue down the tree
        child->Rebuild(context);
    }

    StatelessWidget *GetWidget() const override
    {
        return (StatelessWidget *)widget;
    }

  private:
    Element *child;
    virtual ~StatelessElement()
    {
    }
};
} // namespace GUI
} // namespace Vultr

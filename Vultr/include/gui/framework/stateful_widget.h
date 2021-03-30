#pragma once
#include "widget.h"
#include "element.h"

namespace Vultr
{
namespace GUI
{
class State
{
  public:
    virtual Widget *Build(BuildContext *context) = 0;
};
template <typename T> class StatefulWidget : public Widget
{
  public:
    class StatefulElement : public Element
    {
      public:
        StatefulElement(BuildContext *context, Widget *p_widget)
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
            assert(child != nullptr && "No child for this stateful widget!");
            return child->Layout(context, constraints);
        }

        void Rebuild(BuildContext *context) override
        {
            Widget *build_res = GetWidget()->Build(context);
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
                child = build_res->CreateElement(context);
            }
            // Continue down the tree
            child->Rebuild(context);
        }

        StatefulWidget<T> *GetWidget() const override
        {
            return (StatefulWidget<T> *)widget;
        }

      private:
        Element *child = nullptr;
        virtual ~StatefulElement()
        {
        }
    };
    StatefulWidget(Key key, T *state)
    {
        this->key = key;
        this->state = state;
    }
    Widget *Build(BuildContext *context)
    {
        return GetState().Build(context);
    }
    Element *CreateElement(BuildContext *context) override
    {
        return new StatefulElement(context, this);
    }
    WidgetTypeGetter(StatefulWidget);

  protected:
    T &GetState()
    {
        return *state;
    }
    virtual ~StatefulWidget()
    {
        delete state;
    }

  private:
    T *state;
};
} // namespace GUI
} // namespace Vultr

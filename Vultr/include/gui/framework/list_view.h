#pragma once
#include "render_object.h"
#include "render_object_widget.h"
#include "element.h"
#include <functional>
#define LISTVIEW_PADDING 40

namespace Vultr
{
namespace GUI
{
namespace List
{
struct ChildDimensions
{
    ChildDimensions() : offset(0), height(0)
    {
    }
    double offset;
    double height;
};
struct ElementWidget
{
    ElementWidget() : element(nullptr), widget(nullptr)
    {
    }
    ElementWidget(Widget *p_widget, Element *p_element)
        : element(p_element), widget(p_widget)
    {
    }
    Element *element = nullptr;
    Widget *widget = nullptr;
    ChildDimensions dimensions;
    void Delete(BuildContext *context)
    {
        // element->DeleteElement(context);
        // delete widget;
    }
};
} // namespace List
class ListViewRenderObject : public RenderObject
{
  public:
    ListViewRenderObject(Widget *widget);

    void DeleteRenderObject(BuildContext *context) override;
    ~ListViewRenderObject();

    Size Layout(BuildContext *context, BoxConstraints constraints,
                std::map<int, List::ElementWidget> &children);

    void Paint(BuildContext *context) override;

    void ApplyPosition(BuildContext *context, int index);
    void UpdateReceiver(BuildContext *context);

    Size GetCachedSize()
    {
        return GetSize();
    }

    double scroll_pos = 0;

    bool LayedOut() const
    {
        return layed_out;
    }

  private:
    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert("Incorrect method called!");
        return constraints.Min();
    }
    InputReceiver *receiver = nullptr;
    int cached_z_index = 0;
    bool layed_out = false;
};

class ListView : public RenderObjectWidget
{
  public:
    typedef std::function<Widget *(BuildContext *context, int index)> Builder;

  private:
    struct Params
    {
        Key key;
        Builder builder;
        unsigned int count;
    };

  public:
    Builder builder;
    unsigned int count;
    ListView(Params params);

    const std::vector<Widget *> &GetChildren()
    {
        return children;
    }

    Element *CreateElement(BuildContext *context) override;

    ListViewRenderObject *CreateRenderObject(BuildContext *context) override;

    WidgetTypeGetter(ListView);

  protected:
    std::vector<Widget *> children;
};

class ListViewElement : public RenderObjectElement
{
  public:
    ListViewElement(BuildContext *context, ListView *p_widget);

    void DeleteElement(BuildContext *context) override;

    Size Layout(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No renderobject attached to this element");
        return ((ListViewRenderObject *)render_object)
            ->Layout(context, constraints, children);
    }

    Size GetSize(BuildContext *context, BoxConstraints constraints) override
    {
        assert(render_object != nullptr &&
               "No render object attached to this element");
        return ((ListViewRenderObject *)render_object)
            ->Layout(context, constraints, children);
    }

    void Update(BuildContext *context) override;

    void Reattach(Widget *self) override;

    void Rebuild(BuildContext *context) override;

    ListView *GetWidget() const override
    {
        return (ListView *)widget;
    }

    ListViewRenderObject *GetRenderObject() const
    {
        return (ListViewRenderObject *)render_object;
    }

  protected:
    virtual ~ListViewElement();
    std::map<int, List::ElementWidget> children;
    double scroll_pos = 0;
};

} // namespace GUI
} // namespace Vultr

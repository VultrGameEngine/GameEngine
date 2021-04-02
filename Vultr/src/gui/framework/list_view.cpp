#include <gui/framework/list_view.h>
#include <core/system_providers/render_system_provider.h>
#include <math/lerp.h>

namespace Vultr
{
namespace GUI
{
ListViewRenderObject::ListViewRenderObject(Widget *p_widget) : RenderObject(p_widget)
{
    receiver = new InputReceiver({});
    receiver->on_scroll = [this](ScrollEvent event) {
        // if (event.scroll_amount.y == 0)
        //     return false;
        scroll_pos -= event.scroll_amount.y * 20;
        // Don't scroll past top
        if (scroll_pos < 0)
            scroll_pos = 0;
        MarkForRepaint();
        return true;
    };
}

void ListViewRenderObject::DeleteRenderObject(BuildContext *context)
{
    context->DeleteInputReceiver(cached_z_index);
    delete receiver;
}

ListViewRenderObject::~ListViewRenderObject()
{
}

Size ListViewRenderObject::Layout(BuildContext *context, BoxConstraints constraints,
                                  std::map<int, List::ElementWidget> &children)
{
    layed_out = true;
    return UpdateSize(constraints.Max());
}

void ListViewRenderObject::ApplyPosition(BuildContext *context, int index)
{
}

void ListViewRenderObject::Paint(BuildContext *context)
{
    UpdateReceiver(context);
    // MarkForRepaint();
    repaint_required = false;
}
void ListViewRenderObject::UpdateReceiver(BuildContext *context)
{
    cached_z_index = context->zindex.top();
    glm::vec2 offset = RenderSystemProvider::GetDimensions(GAME) / glm::vec2(2);
    glm::vec2 size = RenderSystemProvider::GetDimensions(GAME);
    receiver->top_left =
        (glm::vec2(context->GetPosition().x - GetSize().width / 2,
                   context->GetPosition().y + GetSize().height / 2) +
         offset) /
        size;
    receiver->bottom_right =
        (glm::vec2(context->GetPosition().x + GetSize().width / 2,
                   context->GetPosition().y - GetSize().height / 2) +
         offset) /
        size;
    context->SubmitInputReceiver(cached_z_index, receiver);
}

ListView::ListView(Params params) : builder(params.builder), count(params.count)
{
    this->key = params.key;
}

Element *ListView::CreateElement(BuildContext *context)
{
    return new ListViewElement(context, this);
}

ListViewRenderObject *ListView::CreateRenderObject(BuildContext *context)
{
    return new ListViewRenderObject(this);
}
ListViewElement::ListViewElement(BuildContext *context, ListView *p_widget)
    : RenderObjectElement(context, p_widget)
{
}

void ListViewElement::DeleteElement(BuildContext *context)
{
}

void ListViewElement::Update(BuildContext *context)
{
    if (render_object->NeedsRepaint(context))
    {
        render_object->Paint(context);
    }
    scroll_pos = Math::Lerp(scroll_pos, GetRenderObject()->scroll_pos, 0.0000000001,
                            context->GetTickInfo().m_delta_time);
    // std::cout << "Scroll pos " << GetRenderObject()->scroll_pos << std::endl;

    if (!GetRenderObject()->LayedOut())
        return;
    for (auto [index, child] : children)
    {
        context->Branch();

        context->AccumulatePosition(glm::vec2(
            context->GetPosition().x,
            context->GetPosition().y +
                GetRenderObject()->GetCachedSize().height / 2 + scroll_pos -
                child.dimensions.height / 2 + child.dimensions.offset));
        child.element->Update(context);
        context->ExitBranch();
    }
}

void ListViewElement::Reattach(Widget *self)
{
    widget = self;
    render_object->Reattach(widget);
    render_object->MarkForRepaint();
}

// This build method is special because it will build and lay out its children
void ListViewElement::Rebuild(BuildContext *context)
{
    if (GetRenderObject() == nullptr || !GetRenderObject()->LayedOut())
        return;

    // Check to make sure within the boundaries
    // for (auto [index, child] : children)
    // {
    //     // If the bottom edge is too far up the screen, we can safely delete it
    //     if (GetRenderObject()->scroll_pos + child.dimensions.offset -
    //             child.dimensions.height >
    //         LISTVIEW_PADDING)
    //     {
    //         child.Delete(context);
    //         children.erase(index);
    //     }
    //     else if (GetRenderObject()->scroll_pos + child.dimensions.offset <
    //              -LISTVIEW_PADDING - GetRenderObject()->GetCachedSize().height)
    //     {
    //         child.Delete(context);
    //         children.erase(index);
    //     }
    // }
    int top_index;
    double offset;
    if (children.size() == 0)
    {
        top_index = 0;
        offset = 0;
    }
    else
    {
        top_index = children.begin()->first;
        offset = children.begin()->second.dimensions.offset;
    }

    int index = top_index;

    BoxConstraints constraints =
        BoxConstraints::Tight(GetRenderObject()->GetCachedSize());
    constraints.min_height = 0;
    constraints.max_height = INFINITY;
    while (index < GetWidget()->count &&
           offset + scroll_pos >
               -GetRenderObject()->GetCachedSize().height - LISTVIEW_PADDING)
    {
        // If we've already built this widget, no need to do it again
        if (index >= children.size())
        {
            Widget *build_res = GetWidget()->builder(context, index);
            Element *element = build_res->CreateElement(context);
            element->Rebuild(context);
            children[index] = List::ElementWidget(build_res, element);
        }
        Size size = children.at(index).element->Layout(context, constraints);
        children.at(index).dimensions.height = size.height;
        children.at(index).dimensions.offset = offset;
        offset -= size.height;
        index++;
    }

    // If we still technically had room for more widgets, it means that our count
    // maxed out and that we can no longer increase our scroll_pos
    if (index < GetWidget()->count - 1)
    {
    }
}

ListViewElement::~ListViewElement()
{
}

} // namespace GUI
} // namespace Vultr

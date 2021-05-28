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
                AccumulateScroll(-event.scroll_amount.y * 20);
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

        Size ListViewRenderObject::Layout(BuildContext *context, BoxConstraints constraints, std::map<int, List::ElementWidget> &children)
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
            repaint_required = false;
        }
        void ListViewRenderObject::UpdateReceiver(BuildContext *context)
        {
            cached_z_index = context->zindex.top();
            glm::vec2 offset = RenderSystem::get_dimensions(GAME) / glm::vec2(2);
            glm::vec2 size = RenderSystem::get_dimensions(GAME);
            receiver->top_left = (glm::vec2(context->GetPosition().x - GetSize().width / 2, context->GetPosition().y + GetSize().height / 2) + offset) / size;
            receiver->bottom_right = (glm::vec2(context->GetPosition().x + GetSize().width / 2, context->GetPosition().y - GetSize().height / 2) + offset) / size;
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
        ListViewElement::ListViewElement(BuildContext *context, ListView *p_widget) : RenderObjectElement(context, p_widget)
        {
        }

        void ListViewElement::DeleteElement(BuildContext *context)
        {
            for (auto [index, element] : children)
            {
                element.Delete(context);
            }
            delete this;
        }

        void ListViewElement::Update(BuildContext *context)
        {
            if (render_object->NeedsRepaint(context))
            {
                render_object->Paint(context);
            }
            scroll_pos = Math::lerp(scroll_pos, GetRenderObject()->scroll_pos, 0.0000000001, context->GetTickInfo().m_delta_time);
            if (!std::isinf(max_scroll))
            {
                if (scroll_pos > max_scroll)
                {
                    scroll_pos = max_scroll;
                    GetRenderObject()->scroll_pos = max_scroll;
                }
                else if (scroll_pos < 0)
                {
                    scroll_pos = 0;
                    GetRenderObject()->scroll_pos = 0;
                }
            }

            if (!GetRenderObject()->LayedOut())
                return;
            for (auto [index, child] : children)
            {
                context->Branch();
                context->AccumulatePosition(
                    glm::vec2(context->GetPosition().x, context->GetPosition().y + GetRenderObject()->GetCachedSize().height / 2 + scroll_pos - child.dimensions.height / 2 + child.dimensions.offset));
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
            GetRenderObject()->ApplyScrollAccumulation();

            // Check to make sure within the boundaries
            for (auto it = children.cbegin(), next_it = it; it != children.cend(); it = next_it)
            {
                ++next_it;

                List::ElementWidget child = it->second;
                if (GetRenderObject()->scroll_pos + child.dimensions.offset - child.dimensions.height > LISTVIEW_PADDING)
                {
                    child.Delete(context);
                    children.erase(it);
                }
                else if (GetRenderObject()->scroll_pos + child.dimensions.offset < -LISTVIEW_PADDING - GetRenderObject()->GetCachedSize().height)
                {
                    child.Delete(context);
                    children.erase(it);
                }
            }

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

            BoxConstraints constraints = BoxConstraints::Tight(GetRenderObject()->GetCachedSize());
            constraints.min_height = 0;
            constraints.max_height = INFINITY;

            double last_child_height = index - 1 > top_index ? children.at(index - 1).dimensions.height : 0;
            while (index < GetWidget()->count && offset + GetRenderObject()->scroll_pos + last_child_height > -GetRenderObject()->GetCachedSize().height - LISTVIEW_PADDING)
            {
                // If we've already built this widget, no need to do it again
                if (children.find(index) == children.end())
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

            if (children.size() == 0)
            {
                return;
            }
            else
            {
                top_index = children.begin()->first;
                offset = children.begin()->second.dimensions.offset;
            }

            index = top_index;

            while (index > 0 && children[index].dimensions.offset + GetRenderObject()->scroll_pos < LISTVIEW_PADDING)
            {
                index--;
                Widget *build_res = GetWidget()->builder(context, index);
                Element *element = build_res->CreateElement(context);
                element->Rebuild(context);
                children[index] = List::ElementWidget(build_res, element);
                Size size = children.at(index).element->Layout(context, constraints);
                children.at(index).dimensions.height = size.height;
                children.at(index).dimensions.offset = offset + size.height;
            }

            // If we still technically had room for more widgets, it means that our count
            // maxed out and that we can no longer increase our scroll_pos
            if (children.size() > 0 && children.find(GetWidget()->count - 1) != children.end())
            {
                List::ElementWidget child = children[GetWidget()->count - 1];
                double bottom_edge = GetRenderObject()->scroll_pos + child.dimensions.offset - child.dimensions.height;
                double height = -GetRenderObject()->GetCachedSize().height;

                // If the bottom edge of the last element is above the bottom edge of our
                // scroll window, then we need to limit scroll_pos
                if (bottom_edge > height)
                {
                    max_scroll = -GetRenderObject()->GetCachedSize().height - child.dimensions.offset + child.dimensions.height;
                }
            }
        }

        ListViewElement::~ListViewElement()
        {
        }

    } // namespace GUI
} // namespace Vultr

#pragma once
#include "single_child_render_object_widget.h"
#include "input_receiver.h"
#include <core/system_providers/render_system_provider.h>

namespace Vultr
{
    namespace GUI
    {
        class Input : public SingleChildRenderObjectWidget
        {
          private:
            struct Params
            {
                Key key;
                Widget *child = nullptr;
                OnHoverCallback on_hover;
                OnUnhoverCallback on_unhover;
                OnMouseDownCallback on_mouse_down;
                OnMouseUpCallback on_mouse_up;
            };
            class RenderInput : public SingleChildRenderObject
            {
              public:
                RenderInput(BuildContext *context, Input *p_widget) : SingleChildRenderObject(p_widget)
                {
                    receiver = new InputReceiver({});
                }

                void DeleteRenderObject(BuildContext *context) override
                {
                    context->DeleteInputReceiver(cached_z_index);
                    delete receiver;
                }

                ~RenderInput()
                {
                }

                Input *GetConfig() override
                {
                    return (Input *)configuration;
                }

                Size Layout(BuildContext *context, BoxConstraints constraints, Element *child) override
                {
                    UpdateSize(child->Layout(context, constraints));
                    return GetSize();
                }

                void Paint(BuildContext *context) override
                {
                    UpdateReceiver(context);
                    repaint_required = false;
                }
                void Reattach(Widget *new_config) override
                {
                    this->configuration = new_config;
                    MarkForRepaint();
                    receiver->on_hover = GetConfig()->on_hover;
                    receiver->on_unhover = GetConfig()->on_unhover;
                    receiver->on_mouse_down = GetConfig()->on_mouse_down;
                    receiver->on_mouse_up = GetConfig()->on_mouse_up;
                }

                void UpdateReceiver(BuildContext *context)
                {
                    cached_z_index = context->zindex.top();
                    receiver->on_hover = GetConfig()->on_hover;
                    receiver->on_unhover = GetConfig()->on_unhover;
                    receiver->on_mouse_down = GetConfig()->on_mouse_down;
                    receiver->on_mouse_up = GetConfig()->on_mouse_up;
                    glm::vec2 offset = RenderSystem::get_dimensions(GAME) / glm::vec2(2);
                    glm::vec2 size = RenderSystem::get_dimensions(GAME);
                    receiver->top_left = (glm::vec2(context->GetPosition().x - GetSize().width / 2, context->GetPosition().y + GetSize().height / 2) + offset) / size;
                    receiver->bottom_right = (glm::vec2(context->GetPosition().x + GetSize().width / 2, context->GetPosition().y - GetSize().height / 2) + offset) / size;
                    context->SubmitInputReceiver(cached_z_index, receiver);
                }

              private:
                InputReceiver *receiver = nullptr;
                int cached_z_index = 0;
            };

          public:
            Input(Params params)
            {
                this->key = params.key;
                this->child = params.child;
                this->on_hover = params.on_hover;
                this->on_unhover = params.on_unhover;
                this->on_mouse_down = params.on_mouse_down;
                this->on_mouse_up = params.on_mouse_up;
            }
            SingleChildRenderObject *CreateRenderObject(BuildContext *context) override
            {
                return new RenderInput(context, this);
            }
            WidgetTypeGetter(Input);

            ~Input()
            {
            }

          private:
            OnHoverCallback on_hover;
            OnUnhoverCallback on_unhover;
            OnMouseDownCallback on_mouse_down;
            OnMouseUpCallback on_mouse_up;
        };
    } // namespace GUI
} // namespace Vultr

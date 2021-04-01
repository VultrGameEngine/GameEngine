#include <gui/layouts/test_state_widget.h>
#include <gui/framework/input.h>

using namespace Vultr;

Widget *TestStateWidgetState::Build(BuildContext *context)
{
    return new Vultr::GUI::Input(
        {
            .child = new Container({
                .key = Key("Test"),
                .child = new AnimatedSizedBox({
                    .child = new RichText({
                        .text = GetWidget()->text,
                        .font = GetWidget()->font,
                        .max_lines = 1,
                        .alignment = RichText::TextAlign::center,
                        .size = GetWidget()->font_size,
                    }),
                    .width = GetScale().x *GetWidget()->size.x,
                    .height = GetScale().y *GetWidget()->size.y,
                    .speed = 0.00000001,
                }),
                .color = glm::vec4(252, 152, 3, 255),
                .borders = EdgeInsets::All(10),
                .border_color = glm::vec4(255, 255, 255, GetBorderAlpha()),
            }),
            .on_hover =
                OnHover(
                    [](HoverEvent event, void *obj) {
                        TestStateWidgetState *state =
                            static_cast<TestStateWidgetState *>(obj);
                        state->hovered = true;
                        return true;
                    },
                    this),
            .on_unhover =
                OnUnhover(
                    [](UnhoverEvent event, void *obj) {
                        TestStateWidgetState *state =
                            static_cast<TestStateWidgetState *>(obj);
                        state->hovered = false;
                        return true;
                    },
                    this),
            .on_mouse_down =
                OnMouseDown(
                    [](MouseDownEvent event, void *obj) {
                        TestStateWidgetState *state =
                            static_cast<TestStateWidgetState *>(obj);
                        state->mouse_down = true;
                        return true;
                    },
                    this),
            .on_mouse_up =
                OnMouseUp(
                    [](MouseUpEvent event, void *obj) {
                        TestStateWidgetState *state =
                            static_cast<TestStateWidgetState *>(obj);
                        state->mouse_down = false;
                        return true;
                    },
                    this),
        });
}

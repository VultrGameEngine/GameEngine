#include <gui/layouts/test_state_widget.h>
#include <gui/framework/input.h>

using namespace Vultr;

Widget *TestStateWidgetState::Build(BuildContext *context)
{
    return new Vultr::GUI::Input({
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
            .color = Vec4(252, 152, 3, 255),
            .borders = EdgeInsets::All(10),
            .border_color = Vec4(255, 255, 255, GetBorderAlpha()),
        }),
        .on_hover =
            [this](HoverEvent event) {
                this->hovered = true;
                return true;
            },
        .on_unhover =
            [this](UnhoverEvent event) {
                this->hovered = false;
                return true;
            },
        .on_mouse_down =
            [this](MouseDownEvent event) {
                this->mouse_down = true;
                return true;
            },
        .on_mouse_up =
            [this](MouseUpEvent event) {
                this->mouse_down = false;
                return true;
            },
    });
}

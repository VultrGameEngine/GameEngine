#include <gui/layouts/test_layout.h>
#include <gui/layouts/test_state_widget.h>

using namespace Vultr::GUI;

Window *TestLayout()
{
    return new Window(new RootWidget({}));
}

Widget *RootWidget::Build(BuildContext *context)
{
    // Vultr::Font *font = context->GetFont("res/fonts/RobotoMono-Regular.ttf");
    return new Center({
        .child = new TestStateWidget({}),
    });
    // return new Center({
    //     .child = new Container({
    //         .child = new RichText({
    //             .text = "abcdygp,a!.;:",
    //             .font = "res/fonts/RobotoMono-Regular.ttf",
    //         }),
    //         .color = glm::vec4(1, 0, 0, 1),
    //     }),
    // });
    // return new Row({
    //     .children =
    //         {
    //             new Flex({
    //                 .child = new Container({
    //                     .width = 25,
    //                     .height = 0.75,
    //                     .color = glm::vec4(1, 0, 0, 1),
    //                 }),
    //                 .flex = 1,
    //             }),
    //             // new Flex({
    //             new Container({
    //                 .width = 0.75,
    //                 .height = 0.25,
    //                 .color = glm::vec4(0, 0, 1, 1),
    //             }),
    //             //     .flex = 5,
    //             // }),
    //             // new Container({
    //             //     .width = 100,
    //             //     .height = 50,
    //             //     .color = glm::vec4(0, 1, 0, 1),
    //             // }),
    //         },
    //     .alignment = Alignment::TopRight(),
    // });
}

#include <gui/layouts/test_layout.h>
#include <gui/layouts/test_state_widget.h>
#include <gui/framework/list_view.h>

using namespace Vultr::GUI;

Window *TestLayout()
{
    return new Window(new RootWidget({}));
}

Widget *RootWidget::Build(BuildContext *context)
{
    // return new Container({
    //     .child = new Center({
    //         .child = new Container({
    //             .child = new ListView({
    //                 .builder =
    //                     [](BuildContext *context, int index) {
    //                         return new Center({
    //                             .child = new TestStateWidget({
    //                                 .text = "Join Game",
    //                             }),
    //                         });
    //                     },
    //                 .count = 100,

    //             }),
    //             .width = 1920,
    //             .height = 1080,
    //             .color = Vec4(0),
    //         }),
    //     }),
    //     .color = Vec4(255, 0, 0, 255),
    // });
    return new Center({
        .child = new TestStateWidget({
            .text = "Join Game",
        }),
    });
    // return new Center({
    //     .child = new Container({
    //         .child = new RichText({
    //             .text = "abcdygp,a!.;:",
    //             .font = "fonts/RobotoMono-Regular.ttf",
    //             .color = Vec4(0, 0, 0, 255),
    //         }),
    //         //         .child = new Row({
    //         //     .children =
    //         //         {
    //         //             new Flex({
    //         //                  .child = new Container({
    //         //                     .width = 250,
    //         //                     .height = 750,
    //         //                     .color = Vec4(255, 0, 0, 255),
    //         //                 }),
    //         //                 .flex = 1,
    //         //             }),
    //         //             new Container({
    //         //                 .width = 400,
    //         //                 .height = 400,
    //         //                 .color = Vec4(0, 255, 0,  255),
    //         //             }),
    //         //             new Flex({
    //         //             .child = new Container({
    //         //                 .width = 750,
    //         //                 .height = 250,
    //         //                 .color = Vec4(0, 0, 255, 255),
    //         //             }),
    //         //                 .flex = 5,
    //         //             }),
    //         //         },
    //         //     .alignment = Alignment::Center(),
    //         // }),
    //         // .width = 700,
    //         // .height = 500,
    //         .color = Vec4(255),
    //     }),
    // });
    // return ;
}

#include <gui/framework/single_child_render_object_widget.h>

namespace Vultr
{
namespace GUI
{
Element *SingleChildRenderObjectWidget::CreateElement(BuildContext *context)
{
    return (Element *)new SingleChildElement(context, this);
}

} // namespace GUI
} // namespace Vultr

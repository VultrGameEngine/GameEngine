#include <gui/framework/multi_child_render_object_widget.h>

namespace Vultr
{

namespace GUI
{
Element *MultiChildRenderObjectWidget::CreateElement(BuildContext *context)
{
    return (Element *)new MultiChildElement(context, this);
}
} // namespace GUI
} // namespace Vultr

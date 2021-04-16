#include <gui/framework/stateless_widget.h>

namespace Vultr
{
namespace GUI
{
Element *StatelessWidget::CreateElement(BuildContext *context)
{
    return (Element *)new StatelessElement(context, this);
}

} // namespace GUI
} // namespace Vultr

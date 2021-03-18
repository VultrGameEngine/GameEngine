#pragma once
#include "stateless_widget.h"

namespace Vultr
{
namespace GUI
{
class Window : public StatelessWidget
{
  public:
    Window(Widget *p_child) : child(p_child)
    {
    }

    Widget *Build() override
    {
        return child;
    }

  private:
    Widget *child;
};
} // namespace GUI
} // namespace Vultr

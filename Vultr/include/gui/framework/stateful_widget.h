#pragma once
#include "widget.h"

namespace Vultr::GUI
{
template <typename T> class StatefulWidget : public Widget
{
  public:
    virtual Widget *Build() = 0;
};
}; // namespace Vultr::GUI

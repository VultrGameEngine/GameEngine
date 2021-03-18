#pragma once
#include "widget.h"

namespace Vultr::GUI
{
class RenderObjectWidget : public Widget
{
  public:
    virtual RenderObject *CreateRenderObject(BuildContext *context) = 0;

  protected:
    RenderObject *render_object;
};
} // namespace Vultr::GUI

#pragma once
#include "widget.h"

namespace Vultr
{
namespace GUI
{

class RenderObjectWidget : public Widget
{
  public:
    virtual RenderObject *CreateRenderObject(BuildContext *context) = 0;

  protected:
    RenderObject *render_object;
};
} // namespace GUI
} // namespace Vultr

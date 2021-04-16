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
    RenderObject *render_object = nullptr;
};
} // namespace GUI
} // namespace Vultr

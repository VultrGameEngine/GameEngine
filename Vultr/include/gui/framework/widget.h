#pragma once
#include "render_object.h"
#include "key.h"
#include <memory>

#define WidgetTypeGetter(T)                                                         \
    std::string GetType() override                                                  \
    {                                                                               \
        return #T;                                                                  \
    }

namespace Vultr
{
namespace GUI
{
class Element;
class Widget
{
  public:
    Widget() = default;
    virtual ~Widget()
    {
    }
    Key key;

    virtual Element *CreateElement(BuildContext *context) = 0;
    virtual std::string GetType() = 0;

  protected:
  private:
};
} // namespace GUI
} // namespace Vultr

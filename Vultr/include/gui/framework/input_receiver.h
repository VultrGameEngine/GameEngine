#pragma once
#include <glm/glm.hpp>

namespace Vultr
{

namespace GUI
{
class InputReceiver
{
  public:
    struct HoverEvent
    {
    };
    typedef bool (*OnHover)(HoverEvent, void *);
    struct UnhoverEvent
    {
    };
    typedef bool (*OnUnhover)(UnhoverEvent, void *);
    OnHover on_hover;
    OnUnhover on_unhover;
    glm::vec2 top_left;
    glm::vec2 bottom_right;

  private:
    struct Params
    {
        OnHover on_hover = nullptr;
        OnUnhover on_unhover = nullptr;
    };

  public:
    InputReceiver(Params params)
        : on_hover(params.on_hover), on_unhover(params.on_unhover)
    {
    }

    ~InputReceiver()
    {
    }
};
} // namespace GUI
} // namespace Vultr

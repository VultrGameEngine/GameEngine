#pragma once
#include <rmlui/rmlui.h>

namespace Vultr
{
class Engine;
class RmlUiInterface : public Rml::SystemInterface
{
  public:
    RmlUiInterface(Vultr::Engine *engine);

    double GetElapsedTime() override;

  private:
    Vultr::Engine *engine;
};
} // namespace Vultr

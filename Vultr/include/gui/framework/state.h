#pragma once

namespace Vultr::GUI
{
class State
{
  public:
    virtual bool RequiresRebuild(const State &other) = 0;

  private:
};
} // namespace Vultr::GUI

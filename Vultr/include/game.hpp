#pragma once
namespace Vultr
{
    struct Engine;
}
#include <core/models/update_tick.h>

class Game
{
  public:
    virtual void Init() = 0;
    virtual void Update(const Vultr::UpdateTick &tick) = 0;
    virtual void Flush() = 0;
    virtual ~Game()
    {
    }
};

extern "C"
{
    Game *init(Vultr::Engine *engine);
    void flush(Game *game);
}

#pragma once
namespace Vultr
{
struct UpdateTick;
class Engine;
} // namespace Vultr

class Game
{
  public:
    virtual void Init() = 0;
    virtual void Update(Vultr::UpdateTick tick) = 0;
    virtual void Flush() = 0;
    virtual ~Game()
    {
    }
};

extern "C"
{
    Game *init(Vultr::Engine &engine);
    void flush(Game *game);
}

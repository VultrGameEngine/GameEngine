#pragma once
namespace Brick3D
{
struct UpdateTick;
}

class Vultr;

class Game
{
  public:
    virtual void Init() = 0;
    virtual void Update(Brick3D::UpdateTick tick) = 0;
    virtual void Flush() = 0;
    virtual ~Game()
    {
    }
};

extern "C"
{
    Game *init(Vultr *engine);
    void flush(Game *game);
}

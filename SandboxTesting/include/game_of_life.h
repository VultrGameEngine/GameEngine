#pragma once
#include <Vultr.hpp>
#define ROWS 16
#define COLS 16
#define ZOLS 16

class GameOfLife : public Game
{
  public:
    GameOfLife(Vultr::Engine *engine)
    {
        this->engine = engine;
    }
    void Init() override;
    void Update(Vultr::UpdateTick tick) override;
    void Flush() override;

  private:
    Vultr::Engine *engine;
};

extern "C"
{
    Game *init(Vultr::Engine *engine);
    void flush(Game *game);
};

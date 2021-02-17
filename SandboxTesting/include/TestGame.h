#pragma once
#include <Vultr.hpp>
#include <stdio.h>

using namespace Vultr;
class TestGame : public Game
{
  public:
    TestGame(Engine *engine)
    {
        std::cout << "Test Game created... " << std::endl;
    }

    void Init() override;

    void Update(Vultr::UpdateTick tick) override
    {
        // std::cout << "Update!" << std::endl;
    }

    void Flush() override
    {
        std::cout << "Shutting down Test Game" << std::endl;
    }

  private:
    Engine *engine;
};

extern "C"
{
    Game *init(Engine *engine);
    void flush(Game *game);
}

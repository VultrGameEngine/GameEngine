#pragma once
#include <Vultr.hpp>

class SandboxTest : public Game
{
  public:
    SandboxTest(Vultr::Engine &p_engine) : engine(p_engine)
    {
    }
    void Init() override;
    void Update(Vultr::UpdateTick tick) override;
    void Flush() override;

  private:
    Vultr::Engine &engine;
};

extern "C"
{
    Game *init(Vultr::Engine &engine);
    void flush(Game *game);
};

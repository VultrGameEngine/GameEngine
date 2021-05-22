#pragma once
#include <vultr.hpp>

class SandboxTest : public Game
{
  public:
    SandboxTest(Vultr::Engine *p_engine) : engine(p_engine)
    {
    }
    void RegisterComponents() override;
    void Init() override;
    void Update(const Vultr::UpdateTick &tick) override;
    void Flush() override;

  private:
    Vultr::Engine *engine;
};

extern "C"
{
    Game *init(Vultr::Engine *engine);
    void flush(Game *game);
};

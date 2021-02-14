#include <Vultr.hpp>
#include <stdio.h>

using namespace Brick3D;
class TestGame : public Game
{
  public:
    TestGame(Vultr *engine)
    {
        std::cout << "Test Game created... " << std::endl;
    }

    void Init() override
    {
        std::cout << "Test Game initialized... " << std::endl;
    }

    void Update(Brick3D::UpdateTick tick) override
    {
        std::cout << "Update!" << std::endl;
    }

    void Flush() override
    {
        std::cout << "Shutting down Test Game" << std::endl;
    }

  private:
    Vultr *engine;
};

extern "C"
{
    Game *init(Vultr *engine)
    {
        return new TestGame(engine);
    }
    void flush(Game *game)
    {
        delete game;
    }
}

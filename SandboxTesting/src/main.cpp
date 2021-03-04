#include <Vultr.hpp>
#include <game_of_life.h>

using namespace Vultr;
int main(void)
{
    Engine *engine = new Engine();

    float lastTime = 0;
    engine->Init(false);
    engine->LoadGame(init(engine));

    while (!engine->should_close)
    {
        engine->UpdateGame(lastTime);
        glfwSwapBuffers(engine->window);
    }
    engine->Destroy();
}

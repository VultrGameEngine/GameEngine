#include <Vultr.hpp>
#include <sandbox_test.h>

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
        glfwPollEvents();
    }
    engine->Destroy();
}

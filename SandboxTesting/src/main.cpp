#include <sandbox_test.h>

using namespace Vultr;
int main(void)
{
    Engine &engine = engine_global();

    float lastTime = 0;
    engine_init(engine, false);
    engine_load_game(engine, init(&engine));

    while (!engine.should_close)
    {
        engine_update_game(engine, lastTime);
        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }
}

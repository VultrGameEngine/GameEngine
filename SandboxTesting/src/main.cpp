#include <sandbox_test.h>

using namespace Vultr;
int main(void)
{
    Engine &engine = engine_global();

    float lastTime = 0;
    engine_init(engine, false);
    engine_load_game(engine, init(&engine));
    engine_init_game(engine);

    while (!glfwWindowShouldClose(engine.window))
    {
        engine_update_game(engine, lastTime);
        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    return 0;
}

#include <sandbox_test.h>
#include <component_registrar.h>

using namespace Vultr;
int main(void)
{
    Engine &engine = engine_global();

    float lastTime = 0;
    engine_init(engine, false);
    engine_load_game(engine, init(&engine));

    change_world(new_world(engine_global().component_registry));
    register_components();

    engine_init_game(engine);

    while (!glfwWindowShouldClose(engine.window))
    {
        engine_update_game(engine, lastTime, true);
        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    return 0;
}

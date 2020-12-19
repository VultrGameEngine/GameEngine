// Controller system handles input from the user translated to transform changes

#include "../../ecs/system/system.hpp"
#include "../components/transform_component.h"
#include "../components/controller_component.h"
#include "../../ecs/world/world.hpp"
#include <memory>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ControllerSystem : public System
{
public:
    void Init(GLFWwindow *window);
    static std::shared_ptr<ControllerSystem> Get();
    void Update(float delta_time);
    void DestroyEntity(Entity entity) override {}
    static std::shared_ptr<ControllerSystem> RegisterSystem();
    static void WindowFocusCallback(GLFWwindow *window, int focused);

private:
    GLFWwindow *window;
    bool focused = false;
};